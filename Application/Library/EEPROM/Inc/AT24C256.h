#ifndef LIBRARY_EEPROM_INC_AT24C256_H_
#define LIBRARY_EEPROM_INC_AT24C256_H_

#include "stdint.h"
#include "stm32h5xx_hal.h"

void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase (uint16_t page);

void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float  fdata);
float EEPROM_Read_NUM (uint16_t page, uint16_t offset);


#endif /* LIBRARY_EEPROM_INC_AT24C256_H_ */
