################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Library/W25Q64JV/Src/W25Q64JV.c 

OBJS += \
./Application/Library/W25Q64JV/Src/W25Q64JV.o 

C_DEPS += \
./Application/Library/W25Q64JV/Src/W25Q64JV.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Library/W25Q64JV/Src/%.o Application/Library/W25Q64JV/Src/%.su Application/Library/W25Q64JV/Src/%.cyclo: ../Application/Library/W25Q64JV/Src/%.c Application/Library/W25Q64JV/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/24C256C/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/W25Q64JV/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Library-2f-W25Q64JV-2f-Src

clean-Application-2f-Library-2f-W25Q64JV-2f-Src:
	-$(RM) ./Application/Library/W25Q64JV/Src/W25Q64JV.cyclo ./Application/Library/W25Q64JV/Src/W25Q64JV.d ./Application/Library/W25Q64JV/Src/W25Q64JV.o ./Application/Library/W25Q64JV/Src/W25Q64JV.su

.PHONY: clean-Application-2f-Library-2f-W25Q64JV-2f-Src

