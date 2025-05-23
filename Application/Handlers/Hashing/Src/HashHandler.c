#include "../../Hashing/Inc/HashHandler.h"
#include "constants.h"
#include "types.h"
#include "Logger.h"
#include <string.h>

#include "stm32h5xx_hal.h"

extern HASH_HandleTypeDef hhash;
extern void Error_Handler(void);

OperationStatus_t HashingHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	log_info("Handling Hashing operation.");
	// Check if either Request or Response Packet is NULL
    if (!request )
    {
        return OPERATION_INVALID_DATA;
    }
    uint32_t AlgorithmSelected;
    // Declare status as Negative for Early Exit Pattern
    OperationStatus_t status = OPERATION_INVALID_OPTION;
    // Declare a static byte array of 64 elements
    static uint8_t digest[64] = {0};
    // Check the Request Packet's option member
    switch (request->option)
    {
    	// SHA224 Hashing Algorithm Selected.
        case OPTION_HASH_SHA224:
        	log_info("SHA224 Hashing Algorithm Selected.");
        	response->outputSize = 28;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA224;
            break;

        // SHA256 Hashing Algorithm Selected.
        case OPTION_HASH_SHA256:
        	log_info("SHA256 Hashing Algorithm Selected.");
        	response->outputSize = 32;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA256;
            break;

        // SHA384 Hashing Algorithm Selected.
        case OPTION_HASH_SHA384:
        	log_info("SHA384 Hashing Algorithm Selected.");
        	response->outputSize = 48;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA384;
            break;

        // SHA512 Hashing Algorithm Selected.
        case OPTION_HASH_SHA512:
        	log_info("SHA512 Hashing Algorithm Selected.");
        	response->outputSize = 64;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA512;
            break;

        // Default switch case
        default:
            return OPERATION_INVALID_OPTION;
    }

    status = OPERATION_SUCCESS;
    HAL_HASH_DeInit(&hhash);

    log_info("De-Initialized HASH Peripheral.");

    hhash.Instance = HASH;
    hhash.Init.DataType = HASH_BYTE_SWAP;
    hhash.Init.Algorithm = AlgorithmSelected;
    if (HAL_HASH_Init(&hhash) != HAL_OK)
    {
    	status = OPERATION_FAILURE;
    }

    log_info("Re-initialized HASH Peripheral.");

    if (HAL_HASH_Start_IT(&hhash, (uint8_t*)request->inputData, (uint32_t)request->inputSize, digest) != HAL_OK)
    {
      Error_Handler();
    }
    while (HAL_HASH_GetState(&hhash) != HAL_HASH_STATE_READY);

    log_info("Generated Hash Digest successfully.");
    // Check if operation status is Success
    if (status != OPERATION_SUCCESS)
    {
        return status;
    }
    log_info("Copying Hash Digest into Response Packet.");
    // Copy digest value to Response Packet structure
    memcpy(response->outputData, digest, response->outputSize);
    log_info("Hash operation complete.");
    // Return Success
    return OPERATION_SUCCESS;
}
