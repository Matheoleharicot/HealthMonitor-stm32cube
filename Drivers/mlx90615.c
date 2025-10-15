#include "stm32l4xx_hal.h"
#include "mlx90615.h"

#include <stdio.h>

#define MLX90615_DEBUG

// EEPROM registers
#define REG_SLAVE_ADDRESS   0x00
#define REG_PWM_T_RANGE     0x01
#define REG_CONFIG          0x02
#define REG_EMISSIVITY      0x03
#define REG_ID0             0x0e
#define REG_ID1             0x0f

// RAM registers
#define REG_RAW_IR_DATA     0x05
#define REG_TA              0x06
#define REG_TO              0x07

// Commands
#define READ_EEPROM(reg)    (0x10 | reg)
#define READ_RAM(reg)       (0x20 | reg)
#define ENTER_SLEEP         0xC6

#define MLX90615_ADDR_WRITE 0xB5
#define MLX90615_ADDR_READ 0xB6

#define MLX90615_EMISSIVITY 0.95


extern I2C_HandleTypeDef hi2c1;
static uint32_t mlx90615PartId = 0;

static void 
mlx90615_write(uint8_t Reg, uint16_t Value)
{
  uint8_t write_data[2];
  write_data[0] = Value & 0xFF;
  write_data[1] = (Value & 0xFF00) >> 8;
  HAL_I2C_Mem_Write(&hi2c1, MLX90615_ADDR_WRITE, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, write_data, 1, 10);
}

static uint16_t 
mlx90615_read(uint8_t Reg)
{
  uint8_t read_data[2];

  HAL_I2C_Mem_Read(&hi2c1, MLX90615_ADDR_READ, Reg, I2C_MEMADD_SIZE_8BIT, read_data, 2, 10);

  return (read_data[1] << 8) | read_data[0];
}

static void 
mlx90615_read_multiple(uint8_t Reg, size_t count, uint8_t *buffer)
{
  HAL_I2C_Mem_Read(&hi2c1, MLX90615_ADDR_READ, Reg, I2C_MEMADD_SIZE_8BIT, buffer, count, 30);
}


void
mlx90615_init(void)
{
  mlx90615PartId = (mlx90615_read(READ_EEPROM(REG_ID0)) << 16) | mlx90615_read(READ_EEPROM(REG_ID1));
  mlx90615_write(READ_EEPROM(REG_EMISSIVITY), (uint16_t)(MLX90615_EMISSIVITY * 16384));
}

uint32_t 
mlx90615_get_id(void)
{
  return mlx90615PartId;
}

float 
mlx90615_read_ambient_temp(void)
{
  // c.f. datasheet p10
  return ((int16_t)mlx90615_read(READ_RAM(REG_TA))) * 0.02 - 273.15;
}

float 
mlx90615_read_temp(void)
{
  // c.f. datasheet p10
  return ((int16_t)mlx90615_read(READ_RAM(REG_TO))) * 0.02 - 273.15;
}
