#include "../../Hashing/Inc/HashHandler.h"
#include "constants.h"
#include "types.h"
#include <string.h>
/*
extern HASH_HandleTypeDef hhash;

static void Compute_HASH_SHA224(void);
static void Compute_HASH_SHA256(void);
static void Compute_HASH_SHA384(void);
static void Compute_HASH_SHA512(void);
static void Compute_HMAC_SHA224(void);
static void Compute_HMAC_SHA256(void);

OperationStatus_t HashingHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	// Check if either Request or Response Packet is NULL
    if (!request || !response)
    {
        return OPERATION_NULL_POINTER;
    }
    // Declare status as Negative for Early Exit Pattern
    OperationStatus_t status = OPERATION_INVALID_OPTION;
    // Declare a static byte array of 64 elements
    static uint8_t digest[64];
    // Check the Request Packet's option member
    switch (request->option)
    {
    	// SHA224 Hashing Algorithm Selected.
        case OPTION_HASH_SHA224:
        	log_info("SHA224 Hashing Algorithm Selected.");
        	response->outputSize = 28;
            break;

        // SHA256 Hashing Algorithm Selected.
        case OPTION_HASH_SHA256:
        	log_info("SHA256 Hashing Algorithm Selected.");
        	response->outputSize = 32;
            break;

        // SHA384 Hashing Algorithm Selected.
        case OPTION_HASH_SHA384:
        	log_info("SHA384 Hashing Algorithm Selected.");
        	response->outputSize = 48;
            break;

        // SHA512 Hashing Algorithm Selected.
        case OPTION_HASH_SHA512:
        	log_info("SHA512 Hashing Algorithm Selected.");
        	response->outputSize = 64;
            break;

        // SHA224 HMAC Algorithm Selected.
        case OPTION_HMAC_SHA224:
        	log_info("SHA224 HMAC Algorithm Selected.");
        	response->outputSize = 28;
            break;

        // SHA256 HMAC Algorithm Selected.
        case OPTION_HMAC_SHA256:
        	log_info("SHA256 HMAC Algorithm Selected.");
        	response->outputSize = 32;
            break;

        // Default switch case
        default:
            return OPERATION_INVALID_OPTION;
    }
    // Check if operation status is Success
    if (status != OPERATION_SUCCESS)
    {
        return status;
    }
    // Copy digest value to Response Packet structure
    memcpy(response->outputData, digest, response->outputSize);
    // Return Success
    return OPERATION_SUCCESS;
}

static void Compute_HASH_SHA224(void)
{
	hhash.Init.DataType = HASH_NO_SWAP;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA224;
	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		Error_Handler();
	}
}
static void Compute_HASH_SHA256(void)
{
	hhash.Init.DataType = HASH_NO_SWAP;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA256;
	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		Error_Handler();
	}
}
static void Compute_HASH_SHA384(void)
{
	hhash.Init.DataType = HASH_NO_SWAP;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA384;
	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		Error_Handler();
	}
}
static void Compute_HASH_SHA512(void)
{
	hhash.Init.DataType = HASH_NO_SWAP;
	hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA512;
	if (HAL_HASH_Init(&hhash) != HAL_OK)
	{
		Error_Handler();
	}
}
static void Compute_HMAC_SHA224(void)
{

}
static void Compute_HMAC_SHA256(void)
{

}*/
