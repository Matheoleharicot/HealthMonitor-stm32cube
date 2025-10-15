################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/NDEF_TagType4_lib/subdir.mk
-include Drivers/M24SR/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Utilities/Fonts/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Drivers/BSP/Components/lsm6dsl/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Drivers/BSP/Components/lps22hb/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Drivers/BSP/Components/lis3mdl/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Drivers/BSP/Components/hts221/subdir.mk
-include Drivers/BSP_B-L475E-IOT01/Drivers/BSP/B-L475E-IOT01/subdir.mk
-include Drivers/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/peripherals/subdir.mk
-include Core/Src/io/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := HealthMonitor-stm32cube
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
HealthMonitor-stm32cube.elf \

MAP_FILES += \
HealthMonitor-stm32cube.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
HealthMonitor-stm32cube.list \


# All Target
all: main-build

# Main-build Target
main-build: HealthMonitor-stm32cube.elf secondary-outputs

# Tool invocations
HealthMonitor-stm32cube.elf HealthMonitor-stm32cube.map: $(OBJS) $(USER_OBJS) /Users/matheomorin/Downloads/HealthMonitor-stm32cube-ISTIC/STM32L475VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "HealthMonitor-stm32cube.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/matheomorin/Downloads/HealthMonitor-stm32cube-ISTIC/STM32L475VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="HealthMonitor-stm32cube.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

HealthMonitor-stm32cube.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "HealthMonitor-stm32cube.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) HealthMonitor-stm32cube.elf HealthMonitor-stm32cube.list HealthMonitor-stm32cube.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
