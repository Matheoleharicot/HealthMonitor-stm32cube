#include "platform.h"
#include "stm32l475e_iot01.h"
#include "max30105.h"

#include <stdio.h>

#define MAX3010X_DEBUG

#define MAX3010X_ADDR_WRITE 0xae
#define MAX3010X_ADDR_READ 0xaf

#define RES_INTERRUPT_STATUS_1 0x00
#define RES_INTERRUPT_STATUS_2 0x01
#define RES_INTERRUPT_ENABLE_1 0x02
#define RES_INTERRUPT_ENABLE_2 0x03
#define RES_FIFO_WRITE_POINTER 0x04
#define RES_OVERFLOW_COUNTER 0x05
#define RES_FIFO_READ_POINTER 0x06
#define RES_FIFO_DATA_REGISTER 0x07
#define RES_FIFO_CONFIGURATION 0x08
#define RES_MODE_CONFIGURATION 0x09
#define RES_SPO2_CONFIGURATION 0x0a
#define RES_LED_PULSE_AMPLITUDE_1 0x0c
#define RES_LED_PULSE_AMPLITUDE_2 0x0d
#define RES_LED_PULSE_AMPLITUDE_3 0x0e
#define RES_PROXIMITY_MODE_LED_PULSE_AMPLITUDE 0x10
#define RES_MULTI_LED_MODE_CONTROL_1 0x11
#define RES_MULTI_LED_MODE_CONTROL_2 0x12
#define RES_DIE_TEMP_INTEGER 0x1f
#define RES_DIE_TEMP_FRACTION 0x20
#define RES_DIE_TEMPERATURE_CONFIG 0x21
#define RES_PROXIMITY_INTERRUPT_THRESHOLD 0x30
#define RES_REVISION_ID 0xfe
#define RES_PART_ID 0xff

extern I2C_HandleTypeDef hi2c1;

#define FS 25    //sampling frequency
#define BUFFER_SIZE (FS * 4) 
#define MA4_SIZE 4 // DONOT CHANGE
#define min(x,y) ((x) < (y) ? (x) : (y))

//uch_spo2_table is approximated as  -45.060*ratioAverage* ratioAverage + 30.354 *ratioAverage + 94.845 ;
const uint8_t uch_spo2_table[184]={ 95, 95, 95, 96, 96, 96, 97, 97, 97, 97, 97, 98, 98, 98, 98, 98, 99, 99, 99, 99, 
              99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
              100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 98, 98, 97, 97, 
              97, 97, 96, 96, 96, 96, 95, 95, 95, 94, 94, 94, 93, 93, 93, 92, 92, 92, 91, 91, 
              90, 90, 89, 89, 89, 88, 88, 87, 87, 86, 86, 85, 85, 84, 84, 83, 82, 82, 81, 81, 
              80, 80, 79, 78, 78, 77, 76, 76, 75, 74, 74, 73, 72, 72, 71, 70, 69, 69, 68, 67, 
              66, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 57, 56, 56, 55, 54, 53, 52, 51, 50, 
              49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 31, 30, 29, 
              28, 27, 26, 25, 23, 22, 21, 20, 19, 17, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 
              3, 2, 1 } ;
static  int32_t an_x[ BUFFER_SIZE]; //ir
static  int32_t an_y[ BUFFER_SIZE]; //red

uint16_t irBuffer[BUFFER_SIZE]; //infrared LED sensor data
uint16_t redBuffer[BUFFER_SIZE];  //red LED sensor data
  

uint32_t bufferLength; //data length
uint32_t bufferCurrent, bufferTotal;
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate; //indicator to show if the heart rate calculation is valid

static void 
max_3010x_write(uint8_t Reg, uint8_t Value)
{
  HAL_I2C_Mem_Write(&hi2c1, MAX3010X_ADDR_WRITE, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&Value, 1, 10);
}

static uint8_t 
max_3010x_read(uint8_t Reg)
{
  uint8_t read_value = 0;

  HAL_I2C_Mem_Read(&hi2c1, MAX3010X_ADDR_READ, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1, 10);

  return read_value;
}

static void 
max_3010x_read_multiple(uint8_t Reg, size_t count, uint8_t *buffer)
{
  HAL_I2C_Mem_Read(&hi2c1, MAX3010X_ADDR_READ, Reg, I2C_MEMADD_SIZE_8BIT, buffer, count, 30);
}

int
max3010x_init()
{
  uint8_t partId = max_3010x_read(RES_PART_ID);
  if (partId != 0x15 // MAX30105
  )
  {
#ifdef MAX3010X_DEBUG
    printf("max3010x_init: invalid id %02x\n", partId);
    return -1;
#endif // MAX3010X_DEBUG
  }
  max_3010x_write(RES_MODE_CONFIGURATION, 0x40); // reset
  while (max_3010x_read(RES_MODE_CONFIGURATION) & 0x40);

  max3010x_getStatus();

  max_3010x_write(RES_INTERRUPT_ENABLE_1, 0x40);
  bufferTotal = 0;
  bufferCurrent = 0;
  
  max_3010x_write(RES_SPO2_CONFIGURATION, 0x25);
  max_3010x_write(RES_LED_PULSE_AMPLITUDE_1, 0x0A); // red
  max_3010x_write(RES_LED_PULSE_AMPLITUDE_2, 0x1F); // IR
  max_3010x_write(RES_LED_PULSE_AMPLITUDE_3, 0x00); // green
  max_3010x_write(RES_PROXIMITY_MODE_LED_PULSE_AMPLITUDE, 0x00);
  max_3010x_write(RES_FIFO_CONFIGURATION, (0x2 << 5) | 0x10);
  max_3010x_write(RES_DIE_TEMPERATURE_CONFIG, 0x01);
  max_3010x_write(RES_MODE_CONFIGURATION, 0x03);
  
  max_3010x_write(RES_FIFO_WRITE_POINTER, 0x00);
  max_3010x_write(RES_OVERFLOW_COUNTER, 0x00);
  max_3010x_write(RES_FIFO_READ_POINTER, 0x00);
  return 0;
}

uint8_t max3010x_getUnreadSampleCount()
{
    uint8_t wr = 0, rd = 0, ovf = 0;
    wr = max_3010x_read(RES_FIFO_WRITE_POINTER);
    rd = max_3010x_read(RES_FIFO_READ_POINTER);
    ovf = max_3010x_read(RES_OVERFLOW_COUNTER);
  
    if (ovf > 0) {
      max3010x_getFIFO(NULL, 32);
      //max_3010x_write(RES_OVERFLOW_COUNTER, 0x00);
      printf("max3010x: overflow (%d)\n", ovf);
      return 0;
    }
  
    if ((wr - rd) < 0)
        return wr - rd + 32;
    else
        return wr - rd;
}

void max3010x_getFIFO(SAMPLE *data, uint8_t sampleCount)
{
    uint8_t dataTemp[31 * 6];
    if (sampleCount > 31)
        sampleCount = 31;
    max_3010x_read_multiple(RES_FIFO_DATA_REGISTER, 6 * sampleCount,
      dataTemp);
    
    if (data) {
      for (uint8_t i = 0; i < sampleCount; i++)
      {
          data[i].red = (((uint32_t)dataTemp[i * 6]) << 16 | ((uint32_t)dataTemp[i * 6 + 1]) << 8 | dataTemp[i * 6 + 2]) & 0x3ffff;
          data[i].iRed = (((uint32_t)dataTemp[i * 6 + 3]) << 16 | ((uint32_t)dataTemp[i * 6 + 4]) << 8 | dataTemp[i * 6 + 5]) & 0x3ffff;
      }
    }
}

uint8_t max3010x_getStatus()
{
    uint8_t data = 0, dataTemp = 0;
    data = max_3010x_read(RES_INTERRUPT_STATUS_1);
    dataTemp = max_3010x_read(RES_INTERRUPT_STATUS_2);
    return data | dataTemp;
}


float max3010x_getTemperature()
{
    int8_t dataTemp = 0;
    float data;
    data = max_3010x_read(RES_DIE_TEMP_INTEGER);
    dataTemp = max_3010x_read(RES_DIE_TEMP_FRACTION);
    data += dataTemp * 0.0625;
 
    max_3010x_write(RES_DIE_TEMPERATURE_CONFIG, 0x01);
    return data;
}

void max3010x_OFF()
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX3010X_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data |= 0x80;
    HAL_I2C_Mem_Write(&hi2c1, MAX3010X_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void max3010x_ON()
{
    uint8_t data = 0;
    HAL_I2C_Mem_Read(&hi2c1, MAX3010X_ADDR_READ, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
    data &= ~(0x80);
    HAL_I2C_Mem_Write(&hi2c1, MAX3010X_ADDR_WRITE, RES_MODE_CONFIGURATION, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
}

void maxim_heart_rate_and_oxygen_saturation(uint16_t *pun_ir_buffer, int32_t n_ir_buffer_length, uint16_t *pun_red_buffer, int32_t *pn_spo2, int8_t *pch_spo2_valid, 
                int32_t *pn_heart_rate, int8_t *pch_hr_valid)
/**
* \brief        Calculate the heart rate and SpO2 level
* \par          Details
*               By detecting  peaks of PPG cycle and corresponding AC/DC of red/infra-red signal, the an_ratio for the SPO2 is computed.
*               Since this algorithm is aiming for Arm M0/M3. formaula for SPO2 did not achieve the accuracy due to register overflow.
*               Thus, accurate SPO2 is precalculated and save longo uch_spo2_table[] per each an_ratio.
*
* \param[in]    *pun_ir_buffer           - IR sensor data buffer
* \param[in]    n_ir_buffer_length      - IR sensor data buffer length
* \param[in]    *pun_red_buffer          - Red sensor data buffer
* \param[out]    *pn_spo2                - Calculated SpO2 value
* \param[out]    *pch_spo2_valid         - 1 if the calculated SpO2 value is valid
* \param[out]    *pn_heart_rate          - Calculated heart rate value
* \param[out]    *pch_hr_valid           - 1 if the calculated heart rate value is valid
*
* \retval       None
*/
{
  uint32_t un_ir_mean ;
  int32_t k, n_i_ratio_count;
  int32_t i, n_exact_ir_valley_locs_count, n_middle_idx;
  int32_t n_th1, n_npks;
  int32_t an_ir_valley_locs[15] ;
  int32_t n_peak_interval_sum;
  
  int32_t n_y_ac, n_x_ac;
  int32_t n_spo2_calc; 
  int32_t n_y_dc_max, n_x_dc_max; 
  int32_t n_y_dc_max_idx, n_x_dc_max_idx; 
  int32_t an_ratio[5], n_ratio_average; 
  int32_t n_nume, n_denom ;


  // calculates DC mean and subtract DC from ir
  un_ir_mean =0; 
  for (k=0 ; k<n_ir_buffer_length ; k++ )
  {
	  un_ir_mean += pun_ir_buffer[k] ;
  }
  un_ir_mean =un_ir_mean/n_ir_buffer_length ;
    
  // remove DC and invert signal so that we can use peak detector as valley detector
  for (k=0 ; k<n_ir_buffer_length ; k++ )  
    an_x[k] = -1*(pun_ir_buffer[k] - un_ir_mean) ; 
    
  // 4 pt Moving Average
  for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
    an_x[k]=( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3])/(int)4;        
  }
  // calculate threshold  
  n_th1=0; 
  for ( k=0 ; k<BUFFER_SIZE ;k++){
    n_th1 +=  an_x[k];
  }
  n_th1=  n_th1/ ( BUFFER_SIZE);
  if( n_th1<30) n_th1=30; // min allowed
  if( n_th1>60) n_th1=60; // max allowed

  for ( k=0 ; k<15;k++) an_ir_valley_locs[k]=0;
  // since we flipped signal, we use peak detector as valley detector
  maxim_find_peaks( an_ir_valley_locs, &n_npks, an_x, BUFFER_SIZE, n_th1, 4, 15 );//peak_height, peak_distance, max_num_peaks 
  n_peak_interval_sum =0;
  if (n_npks>=2){
    for (k=1; k<n_npks; k++) n_peak_interval_sum += (an_ir_valley_locs[k] -an_ir_valley_locs[k -1] ) ;
    n_peak_interval_sum =n_peak_interval_sum/(n_npks-1);
    *pn_heart_rate =(int32_t)( (FS*60)/ n_peak_interval_sum );
    *pch_hr_valid  = 1;
  }
  else  { 
    *pn_heart_rate = -999; // unable to calculate because # of peaks are too small
    *pch_hr_valid  = 0;
  }

  //  load raw value again for SPO2 calculation : RED(=y) and IR(=X)
  for (k=0 ; k<n_ir_buffer_length ; k++ )  {
      an_x[k] =  pun_ir_buffer[k] ; 
      an_y[k] =  pun_red_buffer[k] ;
  }

  // find precise min near an_ir_valley_locs
  n_exact_ir_valley_locs_count =n_npks; 
  
  //using exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration an_ratio
  //finding AC/DC maximum of raw

  n_ratio_average =0; 
  n_i_ratio_count = 0; 
  for(k=0; k< 5; k++) an_ratio[k]=0;
  for (k=0; k< n_exact_ir_valley_locs_count; k++){
    if (an_ir_valley_locs[k] > BUFFER_SIZE ){
      *pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
      *pch_spo2_valid  = 0; 
      return;
    }
  }
  // find max between two valley locations 
  // and use an_ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 
  for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
    n_y_dc_max= -16777216 ; 
    n_x_dc_max= -16777216; 
    if (an_ir_valley_locs[k+1]-an_ir_valley_locs[k] >3){
        for (i=an_ir_valley_locs[k]; i< an_ir_valley_locs[k+1]; i++){
          if (an_x[i]> n_x_dc_max) {n_x_dc_max =an_x[i]; n_x_dc_max_idx=i;}
          if (an_y[i]> n_y_dc_max) {n_y_dc_max =an_y[i]; n_y_dc_max_idx=i;}
      }
      n_y_ac= (an_y[an_ir_valley_locs[k+1]] - an_y[an_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_ir_valley_locs[k]); //red
      n_y_ac=  an_y[an_ir_valley_locs[k]] + n_y_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k])  ; 
      n_y_ac=  an_y[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
      n_x_ac= (an_x[an_ir_valley_locs[k+1]] - an_x[an_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_ir_valley_locs[k]); // ir
      n_x_ac=  an_x[an_ir_valley_locs[k]] + n_x_ac/ (an_ir_valley_locs[k+1] - an_ir_valley_locs[k]); 
      n_x_ac=  an_x[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
      n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
      n_denom= ( n_x_ac *n_y_dc_max)>>7;
      if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0)
      {   
        an_ratio[n_i_ratio_count]= (n_nume*100)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;
        n_i_ratio_count++;
      }
    }
  }
  // choose median value since PPG signal may varies from beat to beat
  maxim_sort_ascend(an_ratio, n_i_ratio_count);
  n_middle_idx= n_i_ratio_count/2;

  if (n_middle_idx >1)
    n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
  else
    n_ratio_average = an_ratio[n_middle_idx ];

  if( n_ratio_average>2 && n_ratio_average <184){
    n_spo2_calc= uch_spo2_table[n_ratio_average] ;
    *pn_spo2 = n_spo2_calc ;
    *pch_spo2_valid  = 1;//  float_SPO2 =  -45.060*n_ratio_average* n_ratio_average/10000 + 30.354 *n_ratio_average/100 + 94.845 ;  // for comparison with table
  }
  else{
    *pn_spo2 =  -999 ; // do not use SPO2 since signal an_ratio is out of range
    *pch_spo2_valid  = 0; 
  }
}


void maxim_find_peaks( int32_t *pn_locs, int32_t *n_npks,  int32_t  *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num )
/**
* \brief        Find peaks
* \par          Details
*               Find at most MAX_NUM peaks above MIN_HEIGHT separated by at least MIN_DISTANCE
*
* \retval       None
*/
{
  maxim_peaks_above_min_height( pn_locs, n_npks, pn_x, n_size, n_min_height );
  maxim_remove_close_peaks( pn_locs, n_npks, pn_x, n_min_distance );
  *n_npks = min( *n_npks, n_max_num );
}

void maxim_peaks_above_min_height( int32_t *pn_locs, int32_t *n_npks,  int32_t  *pn_x, int32_t n_size, int32_t n_min_height )
/**
* \brief        Find peaks above n_min_height
* \par          Details
*               Find all peaks above MIN_HEIGHT
*
* \retval       None
*/
{
  int32_t i = 1, n_width;
  *n_npks = 0;
  
  while (i < n_size-1){
    if (pn_x[i] > n_min_height && pn_x[i] > pn_x[i-1]){      // find left edge of potential peaks
      n_width = 1;
      while (i+n_width < n_size && pn_x[i] == pn_x[i+n_width])  // find flat peaks
        n_width++;
      if (pn_x[i] > pn_x[i+n_width] && (*n_npks) < 15 ){      // find right edge of peaks
        pn_locs[(*n_npks)++] = i;    
        // for flat peaks, peak location is left edge
        i += n_width+1;
      }
      else
        i += n_width;
    }
    else
      i++;
  }
}

void maxim_remove_close_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_min_distance)
/**
* \brief        Remove peaks
* \par          Details
*               Remove peaks separated by less than MIN_DISTANCE
*
* \retval       None
*/
{
    
  int32_t i, j, n_old_npks, n_dist;
    
  /* Order peaks from large to small */
  maxim_sort_indices_descend( pn_x, pn_locs, *pn_npks );

  for ( i = -1; i < *pn_npks; i++ ){
    n_old_npks = *pn_npks;
    *pn_npks = i+1;
    for ( j = i+1; j < n_old_npks; j++ ){
      n_dist =  pn_locs[j] - ( i == -1 ? -1 : pn_locs[i] ); // lag-zero peak of autocorr is at index -1
      if ( n_dist > n_min_distance || n_dist < -n_min_distance )
        pn_locs[(*pn_npks)++] = pn_locs[j];
    }
  }

  // Resort indices int32_to ascending order
  maxim_sort_ascend( pn_locs, *pn_npks );
}

void maxim_sort_ascend(int32_t  *pn_x, int32_t n_size) 
/**
* \brief        Sort array
* \par          Details
*               Sort array in ascending order (insertion sort algorithm)
*
* \retval       None
*/
{
  int32_t i, j, n_temp;
  for (i = 1; i < n_size; i++) {
    n_temp = pn_x[i];
    for (j = i; j > 0 && n_temp < pn_x[j-1]; j--)
        pn_x[j] = pn_x[j-1];
    pn_x[j] = n_temp;
  }
}

void maxim_sort_indices_descend(  int32_t  *pn_x, int32_t *pn_indx, int32_t n_size)
/**
* \brief        Sort indices
* \par          Details
*               Sort indices according to descending order (insertion sort algorithm)
*
* \retval       None
*/ 
{
  int32_t i, j, n_temp;
  for (i = 1; i < n_size; i++) {
    n_temp = pn_indx[i];
    for (j = i; j > 0 && pn_x[n_temp] > pn_x[pn_indx[j-1]]; j--)
      pn_indx[j] = pn_indx[j-1];
    pn_indx[j] = n_temp;
  }
}

void max3010x_cal()
{
  uint8_t unreadSampleCount = max3010x_getUnreadSampleCount();
  if (!unreadSampleCount)
    return;

  SAMPLE sampleBuffTemp[31];
  max3010x_getFIFO(sampleBuffTemp, unreadSampleCount);
  
  uint16_t i, ii;
  //printf("unread=%d\n", unreadSampleCount);
  for (i = 0; i < unreadSampleCount; i++)
  { 
    if (bufferTotal < BUFFER_SIZE)
    {
      redBuffer[bufferCurrent] = (sampleBuffTemp[i].red & 0x3FFFF) >> 2;
      irBuffer[bufferCurrent] = (sampleBuffTemp[i].iRed & 0x3FFFF) >> 2;
      
      ++ bufferTotal;
      ++ bufferCurrent;
      
      if (bufferCurrent >= BUFFER_SIZE)
      {
        maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SIZE, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
        
        for (ii = FS; ii < BUFFER_SIZE; ii++)
        {
          redBuffer[ii - FS] = redBuffer[ii];
          irBuffer[ii - FS] = irBuffer[ii];
        }
        
        bufferCurrent = BUFFER_SIZE-FS;
      }
    }
    else
    {
      redBuffer[bufferCurrent] = (sampleBuffTemp[i].red & 0x3FFFF) >> 2;
      irBuffer[bufferCurrent] = (sampleBuffTemp[i].iRed & 0x3FFFF) >> 2;
      ++ bufferCurrent;
      
      if (bufferCurrent >= BUFFER_SIZE)
      {
        maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SIZE, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
        
        for (ii = FS; ii < BUFFER_SIZE; ii++)
        {
          redBuffer[ii - FS] = redBuffer[ii];
          irBuffer[ii - FS] = irBuffer[ii];
        }
        
        bufferCurrent = BUFFER_SIZE-FS;
      }
    }
  }
}

void 
I2C_MAX30105_ISR(void)
{
  uint8_t intStatus = max3010x_getStatus();
  
  max3010x_INT_Callback(intStatus);
}

__weak void max3010x_INT_Callback(uint8_t intStatus)
{
}

uint8_t max3010x_getHeartRate() { return heartRate; }
uint8_t max3010x_getSpO2() { return spo2; }
int16_t max3010x_getDiff() { return 0; }
uint8_t max3010x_isHeartRateValid(void) { return validHeartRate; }
uint8_t max3010x_isSpO2Valid(void) { return validSPO2; }
