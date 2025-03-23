#ifndef LIBRARY_24C256C_INC_24C256C_H_
#define LIBRARY_24C256C_INC_24C256C_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

#define EEPROM_I2C_ADDR         0x50 << 1 // 7-bit address shifted for HAL
#define EEPROM_PAGE_SIZE        64
#define EEPROM_TOTAL_SIZE       32768

HAL_StatusTypeDef EEPROM_Init(I2C_HandleTypeDef *hi2c);

// Write up to EEPROM_PAGE_SIZE bytes (page write)
HAL_StatusTypeDef EEPROM_WritePage(uint16_t mem_address, uint8_t *data, uint16_t size);

// Read arbitrary number of bytes
HAL_StatusTypeDef EEPROM_Read(uint16_t mem_address, uint8_t *data, uint16_t size);

// Write buffer with page handling
HAL_StatusTypeDef EEPROM_Write(uint16_t mem_address, uint8_t *data, uint16_t size);

#endif /* LIBRARY_24C256C_INC_24C256C_H_ */
