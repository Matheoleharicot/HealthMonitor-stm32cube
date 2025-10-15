#ifndef _I2C_H
#define _I2C_H

#include "stm32l4xx_hal.h"


#ifndef _NOEXTERN
extern
#endif
I2C_HandleTypeDef hi2c1;

#ifndef _NOEXTERN
extern
#endif
I2C_HandleTypeDef hi2c2;


void I2C1_Init(void);

#endif /* _I2C_H */
