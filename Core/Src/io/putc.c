#include "platform.h"
#include "stm32l4xx_hal.h"

#include "../peripherals/uart.h"

#define _NOEXTERN
#include "putc.h"

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
