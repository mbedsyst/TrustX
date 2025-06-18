#include "../../Encryption/Inc/EncryptHandler.h"
#include "CryptoEngine.h"
#include "KeyManager.h"
#include "Generator.h"
#include "Logger.h"
#include "constants.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#include "stm32h5xx_hal.h"

#define CODEC_SUCCESS	1
#define CODEC_FAILURE	0

#define AES_KEY_SIZE	16
#define AES_IV_SIZE		16
#define KEY_STATE_POS	8
#define KEY_DATA_POS	9
#define IV_STATE_POS	25
#define IV_DATA_POS		26
#define PLAINTEXT_POS	42
#define KEYID_LEN		4
#define OUT_KEYID_POS	0
#define OUT_IV_POS		4
#define OUT_CT_POS		20

static uint32_t ConvertKeyIDToUint32(const uint8_t keyID[4])
{
    return ((uint32_t)keyID[0]) |
           ((uint32_t)keyID[1] << 8) |
           ((uint32_t)keyID[2] << 16) |
           ((uint32_t)keyID[3] << 24);
}

OperationStatus_t EncryptHandler_Encrypt(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	log_info("Handling Encryption operation.");
	// Check if either Request or Response Packet is NULL
	if (!request )
	{
		return OPERATION_INVALID_DATA;
	}
	// Initialize variable to store Codec operation status
	int codec_result = 0;
	// Declare variables to assign Key and IV state
	uint8_t keyState, ivState;
	// Initialize variable to store Key ID
	uint8_t keyID[4] = {0};
	uint32_t keyID_32 = 0;
	// Declare array to hold Key and IV data in data section
	static uint8_t keyData[AES_KEY_SIZE], ivData[AES_IV_SIZE];
	// Parse out Key State and IV state from Input Data Stream
	keyState = request->inputData[KEY_STATE_POS];
	ivState = request->inputData[IV_STATE_POS];
	// Calculate the size of input Plaintext
	uint16_t plaintextLen = (request->inputSize) - AES_KEY_SIZE - AES_IV_SIZE - 2;
	// Declare array to hold the output Ciphertext on the stack
	uint8_t *ciphertextData = malloc(plaintextLen);
	// Check if the dynamically allocated array is NULL or not
	if (ciphertextData == NULL)
	{
	    log_error("Failed to allocate buffer for Ciphertext");
	    return OPERATION_UNKNOWN_ERROR;
	}
	// Copy the IV from Data Stream or Generate an IV
	switch(ivState)
	{
		case ENC_IV_BYIV:
			log_info("Using a User-Provided Initialization Vector.");
			memcpy(ivData, &request->inputData[IV_DATA_POS], AES_IV_SIZE);
			break;

		case ENC_IV_GYIV:
			log_info("Generating a Random Initialization Vector");
			GenerateIV(ivData);
			break;

		default:
			log_warn("IV State field not Recognized.");
			break;
	}
	// Copy the Key/Key ID from Data Stream or Generate a Key
	switch(keyState)
	{
		case ENC_KEY_BYOK:
			log_info("Using a User-Provided Encryption Key.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], AES_KEY_SIZE);
			// ToDo Generate a Key ID & Store the provided Key in the Key Manager
			GenerateKEYID(keyID);
			keyID_32 = ConvertKeyIDToUint32(keyID);
			KeyManager_AddKey(keyID_32, keyData, AES_KEY_SIZE, KEY_ORIGIN_PROVIDED, USAGE_ENCRYPT);
			// ToDo Fix datatype of KeyID to uint32_t variable and not uint8_t []
			// ToDo Fix the Key Size parameter
			break;

		case ENC_KEY_DABA:
			log_info("Searching for a stored Key in the Key Manager.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], KEYID_LEN);
			// ToDo Search the Key Manager for a match using the given Key ID
			keyID_32 = ConvertKeyIDToUint32(keyID);
			KeyManager_GetKey(keyID_32, keyData);
			// ToDo Fix datatype of KeyID to uint32_t variable and not uint8_t []
			break;

		case ENC_KEY_GYOK:
			log_info("Generating a Random Encryption Key.");
			GenerateKEY(keyData);
			GenerateKEYID(keyID);
			keyID_32 = ConvertKeyIDToUint32(keyID);
			// ToDo Store the Generated Key in the Key Manager
			KeyManager_AddKey(keyID_32, keyData, AES_KEY_SIZE, KEY_ORIGIN_GENERATED, USAGE_ENCRYPT);
			// ToDo Fix datatype of KeyID to uint32_t variable and not uint8_t []
			// ToDo Fix the Key Size parameter
			break;

		default:
			log_warn("Key State field not Recognized.");
			break;
	}
	// Execute the Encryption operation
	codec_result = CryptoEngine_Codec(ciphertextData,
									  plaintextLen,
									  &request->inputData[PLAINTEXT_POS],
									  plaintextLen,
									  ivData,
									  keyData);
	// Check if Encryption operation was success
	if(codec_result == CODEC_FAILURE)
	{
		log_error("Encryption operation failed.");
		return OPERATION_ENCRYPTION_FAIL;
	}

	// Set the output size in Response Packet
	response->outputSize = plaintextLen + KEYID_LEN + AES_IV_SIZE;
	// Copy the Key ID into Output Data Buffer
	memcpy(&response->outputData[OUT_KEYID_POS], keyID, KEYID_LEN);
	// Copy the Initialization Vector into Output Data Buffer
	memcpy(&response->outputData[OUT_IV_POS], ivData, AES_IV_SIZE);
	// Copy the Ciphertext into Output Data Buffer
	memcpy(&response->outputData[OUT_CT_POS], ciphertextData, plaintextLen);
	// Return the OperationStatus value
	return OPERATION_SUCCESS;
}
