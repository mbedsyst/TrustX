#ifndef PROTOCOL_GENERATOR_INC_GENERATOR_H_
#define PROTOCOL_GENERATOR_INC_GENERATOR_H_

#include <stdint.h>
#include "stm32h5xx_hal.h"  // Or adjust based on your STM32 HAL path

HAL_StatusTypeDef GenerateKEYID(uint8_t *id_out);
HAL_StatusTypeDef GenerateKEY(uint8_t *key_out);
HAL_StatusTypeDef GenerateIV(uint8_t *iv_out);
HAL_StatusTypeDef GenerateSalt(uint8_t *id_out);
HAL_StatusTypeDef GenerateHMAC(uint8_t* input, uint8_t size, uint8_t* key, uint8_t* output);

#endif /* PROTOCOL_GENERATOR_INC_GENERATOR_H_ */
