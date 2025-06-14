#include "../../HMAC/Inc/HMACHandler.h"
#include "Generator.h"
#include "constants.h"
#include "types.h"
#include "Logger.h"
#include <string.h>

#include "stm32h5xx_hal.h"

extern HASH_HandleTypeDef hhash;
extern void Error_Handler(void);

#define KEY_STATE_POS		0
#define KEY_STATE_SIZE		1
#define KEY_DATA_POS		1
#define KEY_DATA_SIZE		32
#define INPUT_DATA_POS		32
#define KEYID_POS			0
#define KEYID_SIZE			4
#define DIGEST_POS			4
#define DIGEST_SIZE_SHA224	28
#define DIGEST_SIZE_SHA256	32

OperationStatus_t HMACHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	log_info("Handling HMAC operation.");
	// Check if either Request or Response Packet is NULL
    if (!request )
    {
        return OPERATION_INVALID_DATA;
    }
	// Declare variable to assign Key state
	uint8_t keyState;
    // Declare variable to assign HMAC Algorithm
    uint32_t AlgorithmSelected;
	// Initialize variable to store Key ID
	uint8_t keyID[KEYID_SIZE] = {0};
	// Pointer to Location of Input Data to Hash
	const uint8_t *inputDataPtr = &request->inputData[INPUT_DATA_POS];
	// Declare array to hold Key and IV data in data section
	static uint8_t keyData[KEY_DATA_SIZE];
    // Declare a static byte array of 64 elements
    static uint8_t digest[DIGEST_SIZE_SHA256] = {0};
	// Parse out Key State and IV state from Input Data Stream
	keyState = request->inputData[KEY_STATE_POS];
	// Calculate the size of input Plaintext
	uint16_t inputDataSize = (request->inputSize) - KEY_STATE_SIZE - KEY_DATA_SIZE;
    // Declare status as Negative for Early Exit Pattern
    OperationStatus_t status = OPERATION_INVALID_OPTION;
    // Check the Request Packet's option member
    switch (request->option)
    {
    	// SHA224 Hashing Algorithm Selected.
        case OPTION_HMAC_SHA224:
        	log_info("SHA224 HMAC Algorithm Selected.");
        	response->outputSize = DIGEST_SIZE_SHA224 + KEYID_SIZE;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA224;
            break;

        // SHA256 Hashing Algorithm Selected.
        case OPTION_HMAC_SHA256:
        	log_info("SHA256 HMAC Algorithm Selected.");
        	response->outputSize = DIGEST_SIZE_SHA256 + KEYID_SIZE;
        	AlgorithmSelected = HASH_ALGOSELECTION_SHA256;
            break;

        // Default switch case
        default:
        	log_error("Option Byte not Recognized.");
            return OPERATION_INVALID_OPTION;
    }
	// Copy the Key/Key ID from Data Stream or Generate a Key
	switch(keyState)
	{
		case HMAC_KEY_BYOK:
			log_info("Using the User-Provided HMAC Key.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], KEY_DATA_SIZE);
			GenerateKEYID(keyID);
			// ToDo Generate a Key ID & Store the provided Key in the Key Manager
			break;

		case HMAC_KEY_DABA:
			log_info("Searching for a stored Key in the Key Manager.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], KEYID_SIZE);
			// ToDo Search the Key Manager for a match using the given Key ID
			break;

		case HMAC_KEY_GYOK:
			log_info("Generating a Random HMAC Key.");
			GenerateKEY(keyData);
			GenerateKEYID(keyID);
			// ToDo Store the Generated Key in the Key Manager
			break;

		default:
			log_warn("Key State field not Recognized.");
			break;
	}

	status = OPERATION_SUCCESS;
    HAL_HASH_DeInit(&hhash);

    log_info("De-Initialized HMAC Peripheral.");

    hhash.Instance = HASH;
    hhash.Init.DataType = HASH_BYTE_SWAP;
    hhash.Init.KeySize = KEY_DATA_SIZE;
    hhash.Init.pKey = (uint8_t *)keyData;
    hhash.Init.Algorithm = HASH_ALGOSELECTION_SHA256;
    if (HAL_HASH_Init(&hhash) != HAL_OK)
    {
      Error_Handler();
    }

    log_info("Re-initialized HASH Peripheral.");

    /* Compute HMAC SHA2 */
    if (HAL_HASH_HMAC_Start(&hhash, inputDataPtr, inputDataSize, digest, 0xFF) != HAL_OK)
    {
        log_error("HMAC Operation Failed.");
        return OPERATION_HMAC_FAIL;
    }

    log_info("Generated HMAC Digest successfully.");
    // Check if operation status is Success
    if (status != OPERATION_SUCCESS)
    {
        return status;
    }

    log_info("Copying HMAC Digest into Response Packet.");
    // Copy Key ID into Response Packet structure
    memcpy(&response->outputData[KEYID_POS], keyID, KEYID_SIZE);
    // Copy HMAC Digest into Response Packet structure
    switch(AlgorithmSelected)
    {
		case OPTION_HMAC_SHA224:
			// Dummy word to zero the last 4-bytes of SHA224 digest
			uint8_t buffer[DIGEST_SIZE_SHA256 -DIGEST_SIZE_SHA224] = {0};
			// Copying the 28-byte Digest into Response Structure
			memcpy(&response->outputData[DIGEST_POS], digest, DIGEST_SIZE_SHA224);
			// Copying zero word to fill last 4-bytes of Digest space in Response Structure
			memcpy(&response->outputData[DIGEST_POS + DIGEST_SIZE_SHA224], buffer, DIGEST_SIZE_SHA256 -DIGEST_SIZE_SHA224);
			break;

		case OPTION_HMAC_SHA256:
			// Copying the 32-byte SHA256 Digest into Response Structure
			memcpy(&response->outputData[DIGEST_POS], digest, DIGEST_SIZE_SHA256);
			break;
    }
    log_info("HMAC operation complete.");
    // Return Success
    return OPERATION_SUCCESS;
}
