#include "platform.h"
#include "drivers.h"

#include "stm32l4xx_hal.h"
#include "stm32l475e_iot01.h"


/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case (GPIO_PIN_1):
    {
      SPI_WIFI_ISR();
      break;
    }
        
    case (NFC_GPIO_GPO_PIN):
    {
      M24SR_GPO_Callback();
      break;
    }
    
    case (GPIO_PIN_13):
    {
      USER_Button_Callback();
      break;
    }
    
    case (GPIO_PIN_14):
    {
      I2C_MAX30105_ISR();
      break;
    }
    default:
    {
      break;
    }
  }
}

__weak void USER_Button_Callback(void)
{
}

void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(NFC_GPIO_GPO_PIN);
}
