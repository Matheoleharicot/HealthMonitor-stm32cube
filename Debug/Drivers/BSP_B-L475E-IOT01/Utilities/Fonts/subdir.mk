################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.c \
../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.c \
../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.c \
../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.c \
../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.c 

OBJS += \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.o \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.o \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.o \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.o \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.o 

C_DEPS += \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.d \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.d \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.d \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.d \
./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/%.o Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/%.su Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/%.cyclo: ../Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/%.c Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP_B-2d-L475E-2d-IOT01-2f-Utilities-2f-Fonts

clean-Drivers-2f-BSP_B-2d-L475E-2d-IOT01-2f-Utilities-2f-Fonts:
	-$(RM) ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.cyclo ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.d ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.o ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font12.su ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.cyclo ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.d ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.o ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font16.su ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.cyclo ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.d ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.o ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font20.su ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.cyclo ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.d ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.o ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font24.su ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.cyclo ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.d ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.o ./Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/font8.su

.PHONY: clean-Drivers-2f-BSP_B-2d-L475E-2d-IOT01-2f-Utilities-2f-Fonts

