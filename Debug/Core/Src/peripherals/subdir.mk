################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/peripherals/clock.c \
../Core/Src/peripherals/exti.c \
../Core/Src/peripherals/gpio.c \
../Core/Src/peripherals/i2c.c \
../Core/Src/peripherals/uart.c 

OBJS += \
./Core/Src/peripherals/clock.o \
./Core/Src/peripherals/exti.o \
./Core/Src/peripherals/gpio.o \
./Core/Src/peripherals/i2c.o \
./Core/Src/peripherals/uart.o 

C_DEPS += \
./Core/Src/peripherals/clock.d \
./Core/Src/peripherals/exti.d \
./Core/Src/peripherals/gpio.d \
./Core/Src/peripherals/i2c.d \
./Core/Src/peripherals/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/peripherals/%.o Core/Src/peripherals/%.su Core/Src/peripherals/%.cyclo: ../Core/Src/peripherals/%.c Core/Src/peripherals/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-peripherals

clean-Core-2f-Src-2f-peripherals:
	-$(RM) ./Core/Src/peripherals/clock.cyclo ./Core/Src/peripherals/clock.d ./Core/Src/peripherals/clock.o ./Core/Src/peripherals/clock.su ./Core/Src/peripherals/exti.cyclo ./Core/Src/peripherals/exti.d ./Core/Src/peripherals/exti.o ./Core/Src/peripherals/exti.su ./Core/Src/peripherals/gpio.cyclo ./Core/Src/peripherals/gpio.d ./Core/Src/peripherals/gpio.o ./Core/Src/peripherals/gpio.su ./Core/Src/peripherals/i2c.cyclo ./Core/Src/peripherals/i2c.d ./Core/Src/peripherals/i2c.o ./Core/Src/peripherals/i2c.su ./Core/Src/peripherals/uart.cyclo ./Core/Src/peripherals/uart.d ./Core/Src/peripherals/uart.o ./Core/Src/peripherals/uart.su

.PHONY: clean-Core-2f-Src-2f-peripherals

