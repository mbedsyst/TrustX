#ifndef PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_
#define PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_

#include "stdint.h"

typedef enum
{
    FLASH_MANAGER_OK = 0,
    FLASH_MANAGER_ERROR = 1,
    FLASH_MANAGER_WRITE_FAIL = 2,
	FLASH_MANAGER_ERASE_FAIL = 3,
    FLASH_MANAGER_INVALID_FLAG = 4,
    FLASH_MANAGER_NOT_INITIALIZED = 5
} FlashManagerStatus_t;

FlashManagerStatus_t FlashManager_Init(void);
FlashManagerStatus_t FlashManager_ReadIdentifier(uint16_t sectorNumber, uint8_t *entryHeader);
FlashManagerStatus_t FlashManager_ReadEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_WriteEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_UpdateEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_InvalidateEntry(uint16_t sectorNumber);
FlashManagerStatus_t FlashManager_EraseEntry(uint16_t sectorNumber);

#endif /* PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_ */
