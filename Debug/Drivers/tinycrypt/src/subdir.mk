################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/tinycrypt/src/aes_decrypt.c \
../Drivers/tinycrypt/src/aes_encrypt.c \
../Drivers/tinycrypt/src/ctr_mode.c \
../Drivers/tinycrypt/src/utils.c 

OBJS += \
./Drivers/tinycrypt/src/aes_decrypt.o \
./Drivers/tinycrypt/src/aes_encrypt.o \
./Drivers/tinycrypt/src/ctr_mode.o \
./Drivers/tinycrypt/src/utils.o 

C_DEPS += \
./Drivers/tinycrypt/src/aes_decrypt.d \
./Drivers/tinycrypt/src/aes_encrypt.d \
./Drivers/tinycrypt/src/ctr_mode.d \
./Drivers/tinycrypt/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tinycrypt/src/%.o Drivers/tinycrypt/src/%.su Drivers/tinycrypt/src/%.cyclo: ../Drivers/tinycrypt/src/%.c Drivers/tinycrypt/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Core/HSMManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Src" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/FlashManager" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Drivers/tinycrypt/inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/CryptoEngine/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/Generator/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/FLASH/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-tinycrypt-2f-src

clean-Drivers-2f-tinycrypt-2f-src:
	-$(RM) ./Drivers/tinycrypt/src/aes_decrypt.cyclo ./Drivers/tinycrypt/src/aes_decrypt.d ./Drivers/tinycrypt/src/aes_decrypt.o ./Drivers/tinycrypt/src/aes_decrypt.su ./Drivers/tinycrypt/src/aes_encrypt.cyclo ./Drivers/tinycrypt/src/aes_encrypt.d ./Drivers/tinycrypt/src/aes_encrypt.o ./Drivers/tinycrypt/src/aes_encrypt.su ./Drivers/tinycrypt/src/ctr_mode.cyclo ./Drivers/tinycrypt/src/ctr_mode.d ./Drivers/tinycrypt/src/ctr_mode.o ./Drivers/tinycrypt/src/ctr_mode.su ./Drivers/tinycrypt/src/utils.cyclo ./Drivers/tinycrypt/src/utils.d ./Drivers/tinycrypt/src/utils.o ./Drivers/tinycrypt/src/utils.su

.PHONY: clean-Drivers-2f-tinycrypt-2f-src

