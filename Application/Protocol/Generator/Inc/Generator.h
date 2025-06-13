#ifndef PROTOCOL_GENERATOR_INC_GENERATOR_H_
#define PROTOCOL_GENERATOR_INC_GENERATOR_H_

#include <stdint.h>
#include "stm32h5xx_hal.h"  // Or adjust based on your STM32 HAL path

/**
 * @brief Generates a 4-byte random key ID using hardware RNG.
 *
 * @param id_out Pointer to a 4-byte buffer to receive the key ID.
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef GenerateKEYID(uint8_t *id_out);

/**
 * @brief Generates a 16-byte AES key using hardware RNG.
 *
 * @param key_out Pointer to a 16-byte buffer to receive the AES key.
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef GenerateKEY(uint8_t *key_out);

/**
 * @brief Generates a 16-byte IV for AES-CTR using hardware RNG.
 *
 * @param iv_out Pointer to a 16-byte buffer to receive the IV.
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef GenerateIV(uint8_t *iv_out);

/**
 * @brief Generates a 32-byte MAC for Key Manager using hardware Hash.
 *
 * @param input Pointer to a input data to hash.
 * @param size Size of input data to hash.
 * @param key Pointer to a 16-byte HMAC Key.
 * @param output Pointer to a 32-byte output buffer.
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef GenerateHMAC(uint8_t* input, uint8_t size, uint8_t* key, uint8_t* output);

#endif /* PROTOCOL_GENERATOR_INC_GENERATOR_H_ */
