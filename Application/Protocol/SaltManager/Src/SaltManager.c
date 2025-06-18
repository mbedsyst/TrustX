#include "SaltManager.h"
#include "Generator.h"
#include "Logger.h"

#include "stm32h5xx_hal.h"
#include <string.h>

static FLASH_EraseInitTypeDef EraseInitStruct;

#define SALT_FLASH_ADDR    ((uint32_t)0x081FFFE0)  // Adjust to an unused last flash page
#define SALT_MAGIC_ADDR    (SALT_FLASH_ADDR)
#define SALT_MAGIC_SIZE	   4
#define SALT_MAGIC_VALUE   0x5A5A5A5A
#define SALT_VALUE_ADDR	   (SALT_MAGIC_ADDR + 4)
#define SALT_VALUE_SIZE	   20

static uint8_t salt[SALT_LENGTH];

static SaltManagerStatus_t WriteSaltToFlash(const uint8_t* salt_data)
{
	if (salt_data == NULL)
	{
		return SALT_MANAGER_SALT_NULL;
	}
	// Variables to hold error status
	HAL_StatusTypeDef status;
    uint32_t SectorError = 0;
    // 32-byte temporary buffer
    uint8_t tempBuf[32] __attribute__((aligned(4)));
    // Configure Flash Erase Init structure
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_2;
    EraseInitStruct.Sector = 127;
    EraseInitStruct.NbSectors = 1;
	// Fill buffer with zeros first
	memset(tempBuf, 0, sizeof(tempBuf));
	// Copy the Magic Word into buffer
	*(uint32_t*)&tempBuf[0] = SALT_MAGIC_VALUE;
	// Copy the 20-byte Salt into buffer
	memcpy(&tempBuf[4], salt_data, 20);
    // Unlock the Flash Interface
    HAL_FLASH_Unlock();
    // Erase Sector 127 of Flash Bank 2
    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
    	log_error("Flash Erase Operation failed.");
    	HAL_FLASH_Lock();
    	return SALT_MANAGER_FLASH_ERASE_FAIL;
    }
    // Program the buffer to Flash (2 Quad-Words)
    for(int i = 0; i < 2; i++)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, (SALT_FLASH_ADDR + i*16), (uint32_t)&tempBuf[0]);
        if (status != HAL_OK)
    	{
        	log_error("Flash Program Operation failed.");
        	HAL_FLASH_Lock();
        	return SALT_MANAGER_FLASH_WRITE_FAIL;
    	}
    }
    return SALT_MANAGER_OK;
}

static void LoadSaltFromFlash(uint8_t* dest)
{
    memcpy(dest, (const void*)SALT_VALUE_ADDR, SALT_VALUE_SIZE);
}

SaltManagerStatus_t SaltManager_Init(void)
{
    if (*(uint32_t*)SALT_MAGIC_ADDR == SALT_MAGIC_VALUE)
    {
        // Salt already initialized
    	LoadSaltFromFlash(salt);
        return SALT_MANAGER_OK;
    }

    // First boot: generate and store
    uint8_t err;
    err = GenerateSalt(salt);
    if(err)
    {
    	log_error("Failed to Generate Salt.");
    	return SALT_MANAGER_GENERATOR_FAIL;
    }
    err = WriteSaltToFlash(salt);
    if(err)
    {
    	log_error("Failed to write Salt to Flash.");
    	return SALT_MANAGER_FLASH_WRITE_FAIL;
    }
    return SALT_MANAGER_OK;
}

const uint8_t* SaltManager_Get(void)
{
    return salt;
}
