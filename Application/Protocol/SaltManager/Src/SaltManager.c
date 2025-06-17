#include "SaltManager.h"
#include "Generator.h"

#include "stm32h5xx_hal.h"
#include <string.h>

static FLASH_EraseInitTypeDef EraseInitStruct;

#define SALT_FLASH_ADDR    ((uint32_t)0x081FF000)  // Adjust to an unused last flash page
#define SALT_MAGIC_ADDR    (SALT_FLASH_ADDR + 0x100)
#define SALT_MAGIC_VALUE   0x5A5A5A5A

static uint8_t salt[SALT_LENGTH];

static void WriteSaltToFlash(const uint8_t* salt_data)
{
    HAL_FLASH_Unlock();

    // ToDo Fill the EraseInitTypeDef structure with options
    //EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    //EraseInitStruct.Banks = (SALT_FLASH_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    //EraseInitStruct.Sector = 0;
    //EraseInitStruct.NbSectors = 1;
    //uint32_t PageError;

    // ToDo Erase the required Page
    //HAL_FLASHEx_Erase(&eraseInit, &PageError);

    // ToDo Program the 20-byte Salt
    //for (uint32_t i = 0; i < SALT_LENGTH; i += 4)
    //{
    //    uint32_t word;
    //    memcpy(&word, &salt_data[i], 4);
    //    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SALT_FLASH_ADDR + i, word);
    //}

    // ToDo Program the 4-byte Magic Value
    //HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SALT_MAGIC_ADDR, SALT_MAGIC_VALUE);

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
