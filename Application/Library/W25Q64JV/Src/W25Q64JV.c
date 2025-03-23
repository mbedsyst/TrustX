#include "W25Q64JV.h"

static SPI_HandleTypeDef *flash_spi = NULL;
static GPIO_TypeDef *flash_cs_port = NULL;
static uint16_t flash_cs_pin = 0;

#define CS_LOW()  HAL_GPIO_WritePin(flash_cs_port, flash_cs_pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(flash_cs_port, flash_cs_pin, GPIO_PIN_SET)

void W25Q64JV_Init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
    flash_spi = hspi;
    flash_cs_port = cs_port;
    flash_cs_pin = cs_pin;
    CS_HIGH();
}

uint8_t W25Q64JV_ReadStatus(void)
{
    uint8_t cmd = W25_CMD_READ_STATUS1;
    uint8_t status = 0;

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(flash_spi, &status, 1, HAL_MAX_DELAY);
    CS_HIGH();

    return status;
}

void W25Q64JV_WriteEnable(void)
{
    uint8_t cmd = W25_CMD_WRITE_ENABLE;

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();
}

void W25Q64JV_WaitForWriteEnd(void)
{
    while (W25Q64JV_ReadStatus() & 0x01)
    {
        HAL_Delay(1);
    }
}

void W25Q64JV_ReadData(uint32_t addr, uint8_t *buf, uint32_t size)
{
    uint8_t cmd[4] = {
        W25_CMD_READ_DATA,
        (addr >> 16) & 0xFF,
        (addr >> 8) & 0xFF,
        addr & 0xFF
    };

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Receive(flash_spi, buf, size, HAL_MAX_DELAY);
    CS_HIGH();
}

void W25Q64JV_PageProgram(uint32_t addr, const uint8_t *buf, uint16_t size)
{
    if (size > W25_PAGE_SIZE)
        size = W25_PAGE_SIZE;

    W25Q64JV_WriteEnable();

    uint8_t cmd[4] = {
        W25_CMD_PAGE_PROGRAM,
        (addr >> 16) & 0xFF,
        (addr >> 8) & 0xFF,
        addr & 0xFF
    };

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, cmd, 4, HAL_MAX_DELAY);
    HAL_SPI_Transmit(flash_spi, (uint8_t *)buf, size, HAL_MAX_DELAY);
    CS_HIGH();

    W25Q64JV_WaitForWriteEnd();
}

void W25Q64JV_SectorErase(uint32_t addr)
{
    W25Q64JV_WriteEnable();

    uint8_t cmd[4] = {
        W25_CMD_SECTOR_ERASE,
        (addr >> 16) & 0xFF,
        (addr >> 8) & 0xFF,
        addr & 0xFF
    };

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, cmd, 4, HAL_MAX_DELAY);
    CS_HIGH();

    W25Q64JV_WaitForWriteEnd();
}

void W25Q64JV_ChipErase(void)
{
    W25Q64JV_WriteEnable();
    uint8_t cmd = W25_CMD_CHIP_ERASE;

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();

    W25Q64JV_WaitForWriteEnd(); // Can take several seconds
}

void W25Q64JV_ReadJEDEC_ID(uint8_t *mf_id, uint8_t *mem_type, uint8_t *capacity)
{
    uint8_t cmd = W25_CMD_JEDEC_ID;
    uint8_t id[3] = {0};

    CS_LOW();
    HAL_SPI_Transmit(flash_spi, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(flash_spi, id, 3, HAL_MAX_DELAY);
    CS_HIGH();

    *mf_id = id[0];
    *mem_type = id[1];
    *capacity = id[2];
}
