#ifndef _UART_H
#define _UART_H

#include "stm32l4xx_hal.h"


#ifndef _NOEXTERN
extern
#endif
UART_HandleTypeDef huart1;


void UART1_Init(void);

#endif /* _UART_H */
