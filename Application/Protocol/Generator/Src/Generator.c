#include "Generator.h"

static HAL_StatusTypeDef Generate_16_bytes(uint8_t *output)
{
    if (output == NULL) {
        return HAL_ERROR;
    }

    RNG_HandleTypeDef hrng = {0};
    hrng.Instance = RNG;

    if (HAL_RNG_Init(&hrng) != HAL_OK) {
        return HAL_ERROR;
    }

    for (int i = 0; i < 16; i += 4) {
        uint32_t rnd;
        if (HAL_RNG_GenerateRandomNumber(&hrng, &rnd) != HAL_OK) {
            HAL_RNG_DeInit(&hrng);
            return HAL_ERROR;
        }

        output[i + 0] = (uint8_t)(rnd >> 24);
        output[i + 1] = (uint8_t)(rnd >> 16);
        output[i + 2] = (uint8_t)(rnd >> 8);
        output[i + 3] = (uint8_t)(rnd);
    }

    HAL_RNG_DeInit(&hrng);
    return HAL_OK;
}

HAL_StatusTypeDef GenerateKEY(uint8_t *key_out)
{
    return Generate_16_bytes(key_out);
}

HAL_StatusTypeDef GenerateIV(uint8_t *iv_out)
{
    return Generate_16_bytes(iv_out);
}

