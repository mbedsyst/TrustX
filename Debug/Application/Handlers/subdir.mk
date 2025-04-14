################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Handlers/DecryptHandler.c \
../Application/Handlers/EncryptHandler.c \
../Application/Handlers/HashHandler.c \
../Application/Handlers/KeyHandler.c \
../Application/Handlers/RandomHandler.c 

OBJS += \
./Application/Handlers/DecryptHandler.o \
./Application/Handlers/EncryptHandler.o \
./Application/Handlers/HashHandler.o \
./Application/Handlers/KeyHandler.o \
./Application/Handlers/RandomHandler.o 

C_DEPS += \
./Application/Handlers/DecryptHandler.d \
./Application/Handlers/EncryptHandler.d \
./Application/Handlers/HashHandler.d \
./Application/Handlers/KeyHandler.d \
./Application/Handlers/RandomHandler.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Handlers/%.o Application/Handlers/%.su Application/Handlers/%.cyclo: ../Application/Handlers/%.c Application/Handlers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Handlers

clean-Application-2f-Handlers:
	-$(RM) ./Application/Handlers/DecryptHandler.cyclo ./Application/Handlers/DecryptHandler.d ./Application/Handlers/DecryptHandler.o ./Application/Handlers/DecryptHandler.su ./Application/Handlers/EncryptHandler.cyclo ./Application/Handlers/EncryptHandler.d ./Application/Handlers/EncryptHandler.o ./Application/Handlers/EncryptHandler.su ./Application/Handlers/HashHandler.cyclo ./Application/Handlers/HashHandler.d ./Application/Handlers/HashHandler.o ./Application/Handlers/HashHandler.su ./Application/Handlers/KeyHandler.cyclo ./Application/Handlers/KeyHandler.d ./Application/Handlers/KeyHandler.o ./Application/Handlers/KeyHandler.su ./Application/Handlers/RandomHandler.cyclo ./Application/Handlers/RandomHandler.d ./Application/Handlers/RandomHandler.o ./Application/Handlers/RandomHandler.su

.PHONY: clean-Application-2f-Handlers

