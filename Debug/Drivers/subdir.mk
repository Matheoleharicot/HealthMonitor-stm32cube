################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/es_wifi.c \
../Drivers/es_wifi_io.c \
../Drivers/max30105.c \
../Drivers/mlx90615.c \
../Drivers/spsgrf.c \
../Drivers/spsgrf_app.c \
../Drivers/spsgrf_io.c \
../Drivers/wifi.c 

OBJS += \
./Drivers/es_wifi.o \
./Drivers/es_wifi_io.o \
./Drivers/max30105.o \
./Drivers/mlx90615.o \
./Drivers/spsgrf.o \
./Drivers/spsgrf_app.o \
./Drivers/spsgrf_io.o \
./Drivers/wifi.o 

C_DEPS += \
./Drivers/es_wifi.d \
./Drivers/es_wifi_io.d \
./Drivers/max30105.d \
./Drivers/mlx90615.d \
./Drivers/spsgrf.d \
./Drivers/spsgrf_app.d \
./Drivers/spsgrf_io.d \
./Drivers/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o Drivers/%.su Drivers/%.cyclo: ../Drivers/%.c Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/es_wifi.cyclo ./Drivers/es_wifi.d ./Drivers/es_wifi.o ./Drivers/es_wifi.su ./Drivers/es_wifi_io.cyclo ./Drivers/es_wifi_io.d ./Drivers/es_wifi_io.o ./Drivers/es_wifi_io.su ./Drivers/max30105.cyclo ./Drivers/max30105.d ./Drivers/max30105.o ./Drivers/max30105.su ./Drivers/mlx90615.cyclo ./Drivers/mlx90615.d ./Drivers/mlx90615.o ./Drivers/mlx90615.su ./Drivers/spsgrf.cyclo ./Drivers/spsgrf.d ./Drivers/spsgrf.o ./Drivers/spsgrf.su ./Drivers/spsgrf_app.cyclo ./Drivers/spsgrf_app.d ./Drivers/spsgrf_app.o ./Drivers/spsgrf_app.su ./Drivers/spsgrf_io.cyclo ./Drivers/spsgrf_io.d ./Drivers/spsgrf_io.o ./Drivers/spsgrf_io.su ./Drivers/wifi.cyclo ./Drivers/wifi.d ./Drivers/wifi.o ./Drivers/wifi.su

.PHONY: clean-Drivers

