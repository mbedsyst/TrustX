#include "../../FlashManager/Inc/FlashManager.h"
#include "constants.h"
#include "Logger.h"
#include "W25Q64.h"
#include <string.h>

#define PAGE_SIZE		256
#define SECTOR_SIZE		4096
#define ENTRY_SIZE		88
#define VALID_FLAG 		0xAA
#define INVALID_FLAG 	0x55
#define IDENTIFIER_SIZE	5

FlashManagerStatus_t FlashManager_Init(void)
{
	uint32_t deviceID = 0;
	log_info("Initializing the Flash Manager.");
	W25Q_Reset();
	deviceID = W25Q_ReadID();
	log_info("External Flash Memory device has been reset.");
	log_info("Flash Memory Device ID : %0xX", deviceID);
	return FLASH_MANAGER_OK;
}

FlashManagerStatus_t FlashManager_ReadIdentifier(uint16_t sectorNumber, uint8_t *entryHeader)
{
	// Calculating Page Number from Sector Number
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	W25Q_Read(page_number, 0, IDENTIFIER_SIZE, entryHeader);
	log_info("Read Sector Identifier from Flash.");
	return FLASH_MANAGER_OK;
}

FlashManagerStatus_t FlashManager_ReadEntry(uint16_t sectorNumber, uint8_t *entry)
{
	// Calculating Page Number and Validity Flag address.
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	uint32_t byte_addr = page_number * PAGE_SIZE;
	// Reading Validity Flag at the start of sector.
	uint8_t entryFlag = W25Q_Read_Byte(byte_addr);
	// Checking if Entry has a Valid Flag or not.
	if (entryFlag == INVALID_FLAG)
	{
		// ToDo Fix Error Handling
		log_error("Entry has Invalid Flag.");
		return FLASH_MANAGER_INVALID_FLAG;
	}
	else if(entryFlag == VALID_FLAG)
	{
		// Reading from the sector if validity flag is 0xAA
		W25Q_Read(page_number, 1, ENTRY_SIZE, entry);
		log_info("Read valid entry from Flash.");
	}
}

FlashManagerStatus_t FlashManager_WriteEntry(uint16_t sectorNumber, uint8_t *entry)
{
	// Calculating Page Number and Validity Flag address
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	uint32_t byte_addr = page_number * PAGE_SIZE;
	// Reading Validity Flag at the start of sector
	uint8_t entryFlag = W25Q_Read_Byte(byte_addr);
	// Checking if Entry has a Valid Flag or not.
	if (entryFlag == VALID_FLAG)
	{
		// ToDo Fix Error Handling
		log_error("Entry has a Valid Flag.");
		log_warn("Cannot write to this sector.");
		return FLASH_MANAGER_ERROR;
	}
	else
	{
		// Writing to the sector if Validity Flag is not 0xAA
		W25Q_Write_Byte(byte_addr, VALID_FLAG);
		W25Q_Write(page_number, 1, ENTRY_SIZE, entry);
		log_info("Wrote a valid entry to Flash.");
	}
}

FlashManagerStatus_t FlashManager_UpdateEntry(uint16_t sectorNumber, uint8_t *entry)
{
	// Calculating Page Number and Validity Flag address
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	uint32_t byte_addr = page_number * PAGE_SIZE;
	// Reading Validity Flag at the start of sector
	uint8_t entryFlag = W25Q_Read_Byte(byte_addr);
	// Checking if Entry has an Invalid Flag or not.
	if (entryFlag == INVALID_FLAG)
	{
		log_error("Entry has an Invalid Flag.");
		return FLASH_MANAGER_ERROR;
	}
	else if(entryFlag == VALID_FLAG)
	{
		// Updating the sector if Validity Flag is 0xAA
		log_info("Erasing the current entry in Flash.");
		W25Q_Erase_Sector(sectorNumber);
		log_info("Updating the current entry in Flash.");
		W25Q_Write_Byte(byte_addr, VALID_FLAG);
		W25Q_Write(page_number, 1, ENTRY_SIZE, entry);
		log_info("Completed updating the Key Entry in Flash.");
	}
}

FlashManagerStatus_t FlashManager_InvalidateEntry(uint16_t sectorNumber)
{
	// Calculating Page Number and Validity Flag address
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	uint32_t byte_addr = page_number * PAGE_SIZE;
	// Writing Invalid Flag to start of Header
	W25Q_Write_Byte(byte_addr, INVALID_FLAG);
	// Reading Validity Flag at the start of sector
	uint8_t entryFlag = W25Q_Read_Byte(byte_addr);
	// Checking if Entry has a Valid Flag or not.
	if (entryFlag == VALID_FLAG)
	{
		log_info("The current entry has a Valid Flag.");
		return FLASH_MANAGER_WRITE_FAIL;
	}
	else
	{
		log_info("The entry has been Invalidated.");
		return FLASH_MANAGER_OK;
	}
}

FlashManagerStatus_t FlashManager_EraseEntry(uint16_t sectorNumber)
{
	// Calculating Page Number and Validity Flag address
	uint32_t page_number = sectorNumber * (SECTOR_SIZE/PAGE_SIZE);
	uint32_t byte_addr = page_number * PAGE_SIZE;
	// Reading Validity Flag at the start of sector
	uint8_t entryFlag = W25Q_Read_Byte(byte_addr);
	// Checking if Entry has a Valid Flag or not.
	if (entryFlag == VALID_FLAG)
	{
		log_info("The current entry has a Valid Flag.");
		W25Q_Erase_Sector(sectorNumber);
		log_info("Completed erasing the Key Entry in Flash.");
	}
	else
	{
		log_info("The current entry has an Invalid Flag.");
		return FLASH_MANAGER_ERROR;
	}
}

