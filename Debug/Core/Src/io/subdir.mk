################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/io/putc.c 

OBJS += \
./Core/Src/io/putc.o 

C_DEPS += \
./Core/Src/io/putc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/io/%.o Core/Src/io/%.su Core/Src/io/%.cyclo: ../Core/Src/io/%.c Core/Src/io/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-io

clean-Core-2f-Src-2f-io:
	-$(RM) ./Core/Src/io/putc.cyclo ./Core/Src/io/putc.d ./Core/Src/io/putc.o ./Core/Src/io/putc.su

.PHONY: clean-Core-2f-Src-2f-io

