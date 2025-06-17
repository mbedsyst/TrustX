#include "SaltManager.h"
#include "Generator.h"

#include "stm32h5xx_hal.h"
#include <string.h>

#define SALT_FLASH_ADDR    ((uint32_t)0x081FF000)  // Adjust to an unused last flash page
#define SALT_MAGIC_ADDR    (SALT_FLASH_ADDR + 0x100)
#define SALT_MAGIC_VALUE   0x5A5A5A5A

static uint8_t salt[SALT_LENGTH];

static void WriteSaltToFlash(const uint8_t* salt_data)
{
    HAL_FLASH_Unlock();
    // Erase the flash page
    FLASH_EraseInitTypeDef eraseInit =
    {
        .TypeErase = FLASH_TYPEERASE_PAGES,
        .Page = (SALT_FLASH_ADDR - 0x08000000) / FLASH_PAGE_SIZE,
        .NbPages = 1
    };
    uint32_t PageError;
    HAL_FLASHEx_Erase(&eraseInit, &PageError);
    // Write salt
    for (uint32_t i = 0; i < SALT_LENGTH; i += 4)
    {
        uint32_t word;
        memcpy(&word, &salt_data[i], 4);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SALT_FLASH_ADDR + i, word);
    }
    // Write magic
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SALT_MAGIC_ADDR, SALT_MAGIC_VALUE);
    HAL_FLASH_Lock();
}

static void LoadSaltFromFlash(uint8_t* dest)
{
    memcpy(dest, (const void*)SALT_FLASH_ADDR, SALT_LENGTH);
}

bool SaltManager_Init(void)
{
    if (*(uint32_t*)SALT_MAGIC_ADDR == SALT_MAGIC_VALUE)
    {
        // Salt already initialized
    	LoadSaltFromFlash(salt);
        return true;
    }

    // First boot: generate and store
    GenerateSalt(salt);
    WriteSaltToFlash(salt);
    return false;
}

const uint8_t* SaltManager_Get(void)
{
    return salt;
}
