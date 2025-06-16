#ifndef PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_
#define PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_

#include "stdint.h"

typedef enum
{
    FLASH_MANAGER_OK = 0,             // Operation successful
    FLASH_MANAGER_ERROR = 1,          // Generic error
    FLASH_MANAGER_WRITE_FAIL = 2,     // Failed to write to flash
    FLASH_MANAGER_READ_FAIL = 3,      // Failed to read from flash
    FLASH_MANAGER_ERASE_FAIL = 4,     // Failed to erase flash
    FLASH_MANAGER_INVALID_FLAG = 5,   // Invalid Flag Value
    FLASH_MANAGER_BUSY = 6,           // Flash is currently busy
    FLASH_MANAGER_TIMEOUT = 7,        // Operation timed out
    FLASH_MANAGER_NOT_INITIALIZED = 8 // FlashManager not initialized
} FlashManagerStatus_t;

FlashManagerStatus_t FlashManager_Init(void);
FlashManagerStatus_t FlashManager_ReadIdentifier(uint16_t sectorNumber, uint8_t *entryHeader);
FlashManagerStatus_t FlashManager_ReadEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_WriteEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_UpdateEntry(uint16_t sectorNumber, uint8_t *entry);
FlashManagerStatus_t FlashManager_InvalidateEntry(uint16_t sectorNumber);
FlashManagerStatus_t FlashManager_EraseEntry(uint16_t sectorNumber);

#endif /* PROTOCOL_FLASHMANAGER_INC_FLASHMANAGER_H_ */
