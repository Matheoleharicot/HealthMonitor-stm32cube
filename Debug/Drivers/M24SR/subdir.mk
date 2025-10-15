################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/M24SR/m24sr.c \
../Drivers/M24SR/m24sr_wrapper.c 

OBJS += \
./Drivers/M24SR/m24sr.o \
./Drivers/M24SR/m24sr_wrapper.o 

C_DEPS += \
./Drivers/M24SR/m24sr.d \
./Drivers/M24SR/m24sr_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/M24SR/%.o Drivers/M24SR/%.su Drivers/M24SR/%.cyclo: ../Drivers/M24SR/%.c Drivers/M24SR/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-M24SR

clean-Drivers-2f-M24SR:
	-$(RM) ./Drivers/M24SR/m24sr.cyclo ./Drivers/M24SR/m24sr.d ./Drivers/M24SR/m24sr.o ./Drivers/M24SR/m24sr.su ./Drivers/M24SR/m24sr_wrapper.cyclo ./Drivers/M24SR/m24sr_wrapper.d ./Drivers/M24SR/m24sr_wrapper.o ./Drivers/M24SR/m24sr_wrapper.su

.PHONY: clean-Drivers-2f-M24SR

