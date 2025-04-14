################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB/Core/Src/subdir.mk
-include USB/Class/CDC/Src/subdir.mk
-include Drivers/STM32H5xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/STM32H5xx_Nucleo/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Application/Utils/Timekeeper/Src/subdir.mk
-include Application/Utils/Logger/Src/subdir.mk
-include Application/Protocol/PacketParser/Src/subdir.mk
-include Application/Protocol/PacketBuilder/Src/subdir.mk
-include Application/Library/FLASH/Src/subdir.mk
-include Application/Library/EEPROM/Src/subdir.mk
-include Application/Handlers/subdir.mk
-include Application/Core/OperationDispatcher/Src/subdir.mk
-include Application/Core/HSMManager/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := HardwareSecurityModule
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
HardwareSecurityModule.elf \

MAP_FILES += \
HardwareSecurityModule.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
HardwareSecurityModule.list \


# All Target
all: main-build

# Main-build Target
main-build: HardwareSecurityModule.elf secondary-outputs

# Tool invocations
HardwareSecurityModule.elf HardwareSecurityModule.map: $(OBJS) $(USER_OBJS) C:\Users\zalma\Downloads\Workspaces\STM32H5_HAL\HardwareSecurityModule\STM32H563ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "HardwareSecurityModule.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"C:\Users\zalma\Downloads\Workspaces\STM32H5_HAL\HardwareSecurityModule\STM32H563ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="HardwareSecurityModule.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

HardwareSecurityModule.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "HardwareSecurityModule.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) HardwareSecurityModule.elf HardwareSecurityModule.list HardwareSecurityModule.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
