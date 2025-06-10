#ifndef PROTOCOL_GENERATOR_INC_GENERATOR_H_
#define PROTOCOL_GENERATOR_INC_GENERATOR_H_

#include <stdint.h>
#include "stm32h5xx_hal.h"  // Or adjust based on your STM32 HAL path

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

#endif /* PROTOCOL_GENERATOR_INC_GENERATOR_H_ */
