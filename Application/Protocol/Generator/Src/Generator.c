#include "Generator.h"

extern RNG_HandleTypeDef hrng;
extern HASH_HandleTypeDef hhash;

static HAL_StatusTypeDef GenerateRandomBytes(uint8_t *output, uint32_t length)
{
    if (output == NULL || length == 0 || (length % 4) != 0) {
        return HAL_ERROR;
    }

    RNG_HandleTypeDef hrng = {0};
    hrng.Instance = RNG;

    if (HAL_RNG_Init(&hrng) != HAL_OK) {
        return HAL_ERROR;
    }

    for (uint32_t i = 0; i < length; i += 4) {
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

HAL_StatusTypeDef GenerateHMAC(uint8_t* input, uint8_t size, uint8_t* key, uint8_t* output)
{
	if (input == NULL || size == 0 || key == 0)
	{
		return HAL_ERROR;
	}
	HAL_HASH_DeInit(&hhash);
	log_info("De-Initialized HASH Peripheral.");
	hhash.Instance = HASH;
	hhash.Init.DataType = HASH_BYTE_SWAP;
	hhash.Init.KeySize = size;
	hhash.Init.pKey = (uint8_t *)key;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA256;
	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_HASH_HMAC_Start(&hhash, (uint8_t *)input, size, (uint8_t *)output, 0xFF) != HAL_OK)
	{
		Error_Handler();
	}
	/* Check the output buffer containing the computing digest with the expected buffer */
	if(memcmp(aDigest, aExpectedDigest ,64) != 0)
	{
		Error_Handler();
	}
}


HAL_StatusTypeDef GenerateKey(uint8_t *key_out)
{
    return GenerateRandomBytes(key_out, 16);
}

HAL_StatusTypeDef GenerateIV(uint8_t *iv_out)
{
    return GenerateRandomBytes(iv_out, 16);
}

HAL_StatusTypeDef GenerateKEYID(uint8_t *id_out)
{
    return GenerateRandomBytes(id_out, 4);
}
