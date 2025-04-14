################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Utils/Timekeeper/Src/Timekeeper.c 

OBJS += \
./Application/Utils/Timekeeper/Src/Timekeeper.o 

C_DEPS += \
./Application/Utils/Timekeeper/Src/Timekeeper.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Utils/Timekeeper/Src/%.o Application/Utils/Timekeeper/Src/%.su Application/Utils/Timekeeper/Src/%.cyclo: ../Application/Utils/Timekeeper/Src/%.c Application/Utils/Timekeeper/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Utils-2f-Timekeeper-2f-Src

clean-Application-2f-Utils-2f-Timekeeper-2f-Src:
	-$(RM) ./Application/Utils/Timekeeper/Src/Timekeeper.cyclo ./Application/Utils/Timekeeper/Src/Timekeeper.d ./Application/Utils/Timekeeper/Src/Timekeeper.o ./Application/Utils/Timekeeper/Src/Timekeeper.su

.PHONY: clean-Application-2f-Utils-2f-Timekeeper-2f-Src

