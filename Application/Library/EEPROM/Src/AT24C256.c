#include "math.h"
#include "string.h"
#include "../Inc/AT24C256.h"

extern I2C_HandleTypeDef hi2c1;
#define EEPROM_I2C 	&hi2c1

#define EEPROM_ADDR 0xA0

#define PAGE_SIZE 	64
#define PAGE_NUM  	512

uint8_t bytes_temp[4];

uint16_t bytestowriteEEPROM (uint16_t size, uint16_t offset)
{
	if ((size+offset)<PAGE_SIZE) return size;
	else return PAGE_SIZE-offset;
}

void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{

	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;
	for (int i=0; i<numofpages; i++)
	{
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowriteEEPROM(size, offset);
		HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000);
		startPage += 1;
		offset=0;
		size = size-bytesremaining;
		pos += bytesremaining;
		HAL_Delay (5);
	}
}

void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union
	{
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++)
    {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }
}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union
	{
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++)
    {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}

void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float data)
{
	float2Bytes(bytes_temp, data);
	EEPROM_Write(page, offset, bytes_temp, 4);
}

float EEPROM_Read_NUM (uint16_t page, uint16_t offset)
{
	uint8_t buffer[4];
	EEPROM_Read(page, offset, buffer, 4);
	return (Bytes2float(buffer));
}

void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;
	for (int i=0; i<numofpages; i++)
	{
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowriteEEPROM(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000);
		startPage += 1;
		offset=0;
		size = size-bytesremaining;
		pos += bytesremaining;
	}
}

void EEPROM_PageErase (uint16_t page)
{
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t MemAddress = page<<paddrposition;
	uint8_t data[PAGE_SIZE];
	memset(data,0xff,PAGE_SIZE);
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, data, PAGE_SIZE, 1000);
	HAL_Delay (5);
}
