################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/NDEF_TagType4_lib/lib_NDEF.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_Email.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_URI.c \
../Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.c \
../Drivers/NDEF_TagType4_lib/lib_TT4_interface.c 

OBJS += \
./Drivers/NDEF_TagType4_lib/lib_NDEF.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.o \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.o \
./Drivers/NDEF_TagType4_lib/lib_TT4_interface.o 

C_DEPS += \
./Drivers/NDEF_TagType4_lib/lib_NDEF.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.d \
./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.d \
./Drivers/NDEF_TagType4_lib/lib_TT4_interface.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/NDEF_TagType4_lib/%.o Drivers/NDEF_TagType4_lib/%.su Drivers/NDEF_TagType4_lib/%.cyclo: ../Drivers/NDEF_TagType4_lib/%.c Drivers/NDEF_TagType4_lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01 -I../Drivers -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-NDEF_TagType4_lib

clean-Drivers-2f-NDEF_TagType4_lib:
	-$(RM) ./Drivers/NDEF_TagType4_lib/lib_NDEF.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF.d ./Drivers/NDEF_TagType4_lib/lib_NDEF.o ./Drivers/NDEF_TagType4_lib/lib_NDEF.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_AAR.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_Email.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_Geo.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_MyApp.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_SMS.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_URI.su ./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.cyclo ./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.d ./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.o ./Drivers/NDEF_TagType4_lib/lib_NDEF_Vcard.su ./Drivers/NDEF_TagType4_lib/lib_TT4_interface.cyclo ./Drivers/NDEF_TagType4_lib/lib_TT4_interface.d ./Drivers/NDEF_TagType4_lib/lib_TT4_interface.o ./Drivers/NDEF_TagType4_lib/lib_TT4_interface.su

.PHONY: clean-Drivers-2f-NDEF_TagType4_lib

