################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.c \
../Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.c 

OBJS += \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.o \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.o 

C_DEPS += \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.d \
./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32H5xx_HAL_Driver/Src/%.o Drivers/STM32H5xx_HAL_Driver/Src/%.su Drivers/STM32H5xx_HAL_Driver/Src/%.cyclo: ../Drivers/STM32H5xx_HAL_Driver/Src/%.c Drivers/STM32H5xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32H563xx -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32H5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Class/CDC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/USB/Core/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Utils/Logger/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketParser/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/PacketBuilder/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Core/HSMManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Src" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Encryption/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/FlashManager" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/CryptoEngine/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/Generator/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/FLASH/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/HMAC/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/FlashManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Library/tinycrypt/inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/KeyDerivator/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/SaltManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/KeyManager/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Protocol/DeviceInfo/Inc" -I"C:/Users/zalma/Downloads/Workspaces/STM32H5_HAL/HardwareSecurityModule/Application/Handlers/Decryption/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_cortex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_dma_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_exti.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_flash_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_gpio.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_hash.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_i2c_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pcd_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_pwr_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rcc_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rng_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_rtc_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_spi_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_uart_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.o
	-$(RM) ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_hal_usart_ex.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_rng.su ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.cyclo ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.d ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.o ./Drivers/STM32H5xx_HAL_Driver/Src/stm32h5xx_ll_usb.su

.PHONY: clean-Drivers-2f-STM32H5xx_HAL_Driver-2f-Src

