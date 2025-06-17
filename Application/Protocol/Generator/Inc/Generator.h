#ifndef PROTOCOL_GENERATOR_INC_GENERATOR_H_
#define PROTOCOL_GENERATOR_INC_GENERATOR_H_

#include <stdint.h>
#include "stm32h5xx_hal.h"  // Or adjust based on your STM32 HAL path

typedef enum
{
	GENERATOR_STATUS_OK = 0,
	GENERATOR_STATUS_INPUT_NULL,
	GENERATOR_STATUS_RNG_INIT_FAIL,
	GENERATOR_STATUS_HASH_INIT_FAIL,
	GENERATOR_STATUS_RNG_OP_FAIL,
	GENERATOR_STATUS_HASH_OP_FAIL
}GeneratorStatus_t;

GeneratorStatus_t GenerateKEYID(uint8_t *id_out);
GeneratorStatus_t GenerateKEY(uint8_t *key_out);
GeneratorStatus_t GenerateIV(uint8_t *iv_out);
GeneratorStatus_t GenerateSalt(uint8_t *id_out);
GeneratorStatus_t GenerateHMAC(uint8_t* input, uint8_t size, uint8_t* key, uint8_t* output);

#endif /* PROTOCOL_GENERATOR_INC_GENERATOR_H_ */
