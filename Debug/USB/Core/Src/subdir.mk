################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB/Core/Src/usbd_conf.c \
../USB/Core/Src/usbd_core.c \
../USB/Core/Src/usbd_ctlreq.c \
../USB/Core/Src/usbd_desc.c \
../USB/Core/Src/usbd_ioreq.c 

OBJS += \
./USB/Core/Src/usbd_conf.o \
./USB/Core/Src/usbd_core.o \
./USB/Core/Src/usbd_ctlreq.o \
./USB/Core/Src/usbd_desc.o \
./USB/Core/Src/usbd_ioreq.o 

C_DEPS += \
./USB/Core/Src/usbd_conf.d \
./USB/Core/Src/usbd_core.d \
./USB/Core/Src/usbd_ctlreq.d \
./USB/Core/Src/usbd_desc.d \
./USB/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
USB/Core/Src/%.o USB/Core/Src/%.su USB/Core/Src/%.cyclo: ../USB/Core/Src/%.c USB/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Core/HSMManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Src" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/FlashManager" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/CryptoEngine/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/Generator/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/FLASH/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/HMAC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/FlashManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/tinycrypt/inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/KeyDerivator/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/SaltManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/KeyManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/DeviceInfo/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Decryption/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-USB-2f-Core-2f-Src

clean-USB-2f-Core-2f-Src:
	-$(RM) ./USB/Core/Src/usbd_conf.cyclo ./USB/Core/Src/usbd_conf.d ./USB/Core/Src/usbd_conf.o ./USB/Core/Src/usbd_conf.su ./USB/Core/Src/usbd_core.cyclo ./USB/Core/Src/usbd_core.d ./USB/Core/Src/usbd_core.o ./USB/Core/Src/usbd_core.su ./USB/Core/Src/usbd_ctlreq.cyclo ./USB/Core/Src/usbd_ctlreq.d ./USB/Core/Src/usbd_ctlreq.o ./USB/Core/Src/usbd_ctlreq.su ./USB/Core/Src/usbd_desc.cyclo ./USB/Core/Src/usbd_desc.d ./USB/Core/Src/usbd_desc.o ./USB/Core/Src/usbd_desc.su ./USB/Core/Src/usbd_ioreq.cyclo ./USB/Core/Src/usbd_ioreq.d ./USB/Core/Src/usbd_ioreq.o ./USB/Core/Src/usbd_ioreq.su

.PHONY: clean-USB-2f-Core-2f-Src

