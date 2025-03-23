#include "24C256C.h"
#include <string.h>

static I2C_HandleTypeDef *eeprom_i2c = NULL;

HAL_StatusTypeDef EEPROM_Init(I2C_HandleTypeDef *hi2c)
{
    eeprom_i2c = hi2c;
    return HAL_OK; // Just storing pointer here, assume I2C already initialized
}

HAL_StatusTypeDef EEPROM_Read(uint16_t mem_address, uint8_t *data, uint16_t size) {
    uint8_t addr_bytes[2] =
    {
        (uint8_t)(mem_address >> 8),
        (uint8_t)(mem_address & 0xFF)
    };

    return HAL_I2C_Mem_Read(eeprom_i2c, EEPROM_I2C_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, data, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef EEPROM_WritePage(uint16_t mem_address, uint8_t *data, uint16_t size) {
    if (size > EEPROM_PAGE_SIZE)
        return HAL_ERROR;

    return HAL_I2C_Mem_Write(eeprom_i2c, EEPROM_I2C_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, data, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef EEPROM_Write(uint16_t mem_address, uint8_t *data, uint16_t size) {
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t bytes_written = 0;

    while (bytes_written < size) {
        uint16_t current_address = mem_address + bytes_written;
        uint16_t page_offset = current_address % EEPROM_PAGE_SIZE;
        uint16_t space_in_page = EEPROM_PAGE_SIZE - page_offset;
        uint16_t bytes_to_write = (size - bytes_written) < space_in_page ? (size - bytes_written) : space_in_page;

        status = EEPROM_WritePage(current_address, &data[bytes_written], bytes_to_write);
        if (status != HAL_OK) {
            return status;
        }

        // Wait for EEPROM to finish internal write (tWR ~ 5ms)
        HAL_Delay(5);

        bytes_written += bytes_to_write;
    }

    return status;
}
