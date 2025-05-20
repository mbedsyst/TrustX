################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Core/HSMManager/Src/HSMManager.c 

OBJS += \
./Application/Core/HSMManager/Src/HSMManager.o 

C_DEPS += \
./Application/Core/HSMManager/Src/HSMManager.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Core/HSMManager/Src/%.o Application/Core/HSMManager/Src/%.su Application/Core/HSMManager/Src/%.cyclo: ../Application/Core/HSMManager/Src/%.c Application/Core/HSMManager/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Core/HSMManager/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Core-2f-HSMManager-2f-Src

clean-Application-2f-Core-2f-HSMManager-2f-Src:
	-$(RM) ./Application/Core/HSMManager/Src/HSMManager.cyclo ./Application/Core/HSMManager/Src/HSMManager.d ./Application/Core/HSMManager/Src/HSMManager.o ./Application/Core/HSMManager/Src/HSMManager.su

.PHONY: clean-Application-2f-Core-2f-HSMManager-2f-Src

