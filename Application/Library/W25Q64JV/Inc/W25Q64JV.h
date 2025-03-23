#ifndef LIBRARY_W25Q64JV_INC_W25Q64JV_H_
#define LIBRARY_W25Q64JV_INC_W25Q64JV_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

// Commands
#define W25_CMD_WRITE_ENABLE      0x06
#define W25_CMD_WRITE_DISABLE     0x04
#define W25_CMD_READ_STATUS1      0x05
#define W25_CMD_PAGE_PROGRAM      0x02
#define W25_CMD_READ_DATA         0x03
#define W25_CMD_SECTOR_ERASE      0x20 // 4KB
#define W25_CMD_BLOCK_ERASE_64K   0xD8
#define W25_CMD_CHIP_ERASE        0xC7
#define W25_CMD_JEDEC_ID          0x9F

#define W25_PAGE_SIZE             256
#define W25_SECTOR_SIZE           4096
#define W25_FLASH_SIZE_BYTES      (8 * 1024 * 1024) // 8MB

// SPI + CS control
void W25Q64JV_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin);

// Basic ops
uint8_t  W25Q64JV_ReadStatus(void);
void     W25Q64JV_WriteEnable(void);
void     W25Q64JV_WaitForWriteEnd(void);

// Read/Write
void     W25Q64JV_ReadData(uint32_t addr, uint8_t *buf, uint32_t size);
void     W25Q64JV_PageProgram(uint32_t addr, const uint8_t *buf, uint16_t size);

// Erase
void     W25Q64JV_SectorErase(uint32_t addr);
void     W25Q64JV_ChipErase(void);

// ID
void     W25Q64JV_ReadJEDEC_ID(uint8_t *mf_id, uint8_t *mem_type, uint8_t *capacity);


#endif /* LIBRARY_W25Q64JV_INC_W25Q64JV_H_ */
