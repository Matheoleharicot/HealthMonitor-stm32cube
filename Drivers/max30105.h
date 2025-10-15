#ifndef __MAX_3010X_H_
#define __MAX_3010X_H_

#include <stdint.h>


#define MAX30102_INTERRUPT_STATUS_ALMOST_FULL 0x80
#define MAX30102_INTERRUPT_STATUS_NEW_FIFO_DATA 0x40
#define MAX30102_INTERRUPT_STATUS_ALC_OVF 0x20
#define MAX30102_INTERRUPT_STATUS_PROX_INT 0x10
#define MAX30102_INTERRUPT_STATUS_TEMPERATURE_READY 0x02
#define MAX30102_INTERRUPT_STATUS_POWER_READY 0x01

typedef struct samplestruct
{
    uint32_t red;
    uint32_t iRed;
} SAMPLE;

int max3010x_init(void);
uint8_t max3010x_getUnreadSampleCount(void);
void max3010x_getFIFO(SAMPLE *data, uint8_t sampleCount);
uint8_t max3010x_getStatus(void);
float max3010x_getTemperature(void);
void max3010x_OFF(void);
void max3010x_ON(void);

void max3010x_cal(void);
uint8_t max3010x_getHeartRate(void);
uint8_t max3010x_getSpO2(void);
uint8_t max3010x_isHeartRateValid(void);
uint8_t max3010x_isSpO2Valid(void);
int16_t max3010x_getDiff(void);
void I2C_MAX30105_ISR(void);
void max3010x_INT_Callback(uint8_t intStatus);

void maxim_heart_rate_and_oxygen_saturation(uint16_t *pun_ir_buffer, int32_t n_ir_buffer_length, uint16_t *pun_red_buffer, int32_t *pn_spo2, int8_t *pch_spo2_valid, int32_t *pn_heart_rate, int8_t *pch_hr_valid);
void maxim_find_peaks(int32_t *pn_locs, int32_t *n_npks,  int32_t  *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num);
void maxim_peaks_above_min_height(int32_t *pn_locs, int32_t *n_npks,  int32_t  *pn_x, int32_t n_size, int32_t n_min_height);
void maxim_remove_close_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_min_distance);
void maxim_sort_ascend(int32_t  *pn_x, int32_t n_size);
void maxim_sort_indices_descend(int32_t  *pn_x, int32_t *pn_indx, int32_t n_size);
#endif
