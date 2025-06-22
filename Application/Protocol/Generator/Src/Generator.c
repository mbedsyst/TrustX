#include "Generator.h"
#include "Logger.h"

extern RNG_HandleTypeDef hrng;
extern HASH_HandleTypeDef hhash;

#define HMAC_KEY_SIZE	16

static GeneratorStatus_t GenerateRandomBytes(uint8_t *output, uint32_t length)
{
    if (output == NULL || length == 0 || (length % 4) != 0)
    {
        return GENERATOR_STATUS_INPUT_NULL;
    }

    HAL_RNG_DeInit(&hrng);
    log_info("De-Initialized RNG Peripheral.");
    hrng.Instance = RNG;
    hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;

    if (HAL_RNG_Init(&hrng) != HAL_OK)
    {
    	log_error("Failed to Re-Initialize the RNG Peripheral.");
    	return GENERATOR_STATUS_RNG_INIT_FAIL;
    }

    log_info("Re-Initialized RNG Peripheral.");

    for (uint32_t i = 0; i < length; i += 4)
    {
        uint32_t rnd;
        if (HAL_RNG_GenerateRandomNumber(&hrng, &rnd) != HAL_OK)
        {
        	log_error("RNG Operation failed.");
            return GENERATOR_STATUS_RNG_OP_FAIL;
        }

        output[i + 0] = (uint8_t)(rnd >> 24);
        output[i + 1] = (uint8_t)(rnd >> 16);
        output[i + 2] = (uint8_t)(rnd >> 8);
        output[i + 3] = (uint8_t)(rnd);
    }

    log_info("RNG Generated for %d bytes.", length);
    return GENERATOR_STATUS_OK;
}

GeneratorStatus_t GenerateHMAC(uint8_t* input, uint8_t size, uint8_t* key, uint8_t* output)
{
	if (input == NULL || size == 0 || key == 0)
	{
		return GENERATOR_STATUS_INPUT_NULL;
	}

	HAL_HASH_DeInit(&hhash);
	log_info("De-Initialized HASH Peripheral.");

	hhash.Instance = HASH;
	hhash.Init.DataType = HASH_BYTE_SWAP;
	hhash.Init.KeySize = HMAC_KEY_SIZE;
	hhash.Init.pKey = (uint8_t *)key;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA256;

	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		log_error("Failed to Re-Initialize the Hash Peripheral.");
		return GENERATOR_STATUS_HASH_INIT_FAIL;
	}

	log_info("Re-Initialized HASH Peripheral.");

	if (HAL_HASH_HMAC_Start(&hhash, (uint8_t *)input, size, (uint8_t *)output, 0xFF) != HAL_OK)
	{
		log_error("HMAC Operation failed.");
		return GENERATOR_STATUS_HASH_OP_FAIL;
	}

	log_info("HMAC Generated for Key Blob.");
	return GENERATOR_STATUS_OK;
}


GeneratorStatus_t GenerateKey(uint8_t *key_out)
{
    return GenerateRandomBytes(key_out, 16);
}

GeneratorStatus_t GenerateIV(uint8_t *iv_out)
{
    return GenerateRandomBytes(iv_out, 16);
}

GeneratorStatus_t GenerateKEYID(uint8_t *id_out)
{
    return GenerateRandomBytes(id_out, 4);
}

GeneratorStatus_t GenerateSalt(uint8_t *id_out)
{
    return GenerateRandomBytes(id_out, 20);
}
