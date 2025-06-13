#include "../../Decryption/Inc/DecryptHandler.h"
#include "CryptoEngine.h"
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
#define OUT_CT_POS		4

OperationStatus_t DecryptHandler_Decrypt(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	log_info("Handling Decryption operation.");
	// Check if either Request or Response Packet is NULL
	if (!request )
	{
		return OPERATION_INVALID_DATA;
	}
	// Initialize variable to store Codec operation status
	int codec_result = 0;
	// Declare variables to assign Key and IV state
	uint8_t keyState, ivState;
	// Initialize array to store Key ID
	uint8_t keyID[4] = {0};
	// Declare array to hold Key and IV data in data section
	static uint8_t keyData[AES_KEY_SIZE], ivData[AES_IV_SIZE];
	// Parse out Key State and IV state from Input Data Stream
	keyState = request->inputData[KEY_STATE_POS];
	ivState = request->inputData[IV_STATE_POS];
	// Calculate the size of input Ciphertext
	const uint16_t ciphertextLen = (request->inputSize) - AES_KEY_SIZE - AES_IV_SIZE - 2;
	// Declare array to hold the output Ciphertext on the stack
	uint8_t *plaintextData = malloc(ciphertextLen);
	// Check if the dynamically allocated array is NULL or not
	if (plaintextData == NULL)
	{
	    log_error("Failed to allocate buffer for Plaintext");
	    return OPERATION_UNKNOWN_ERROR;
	}
	// Copy the Key/Key ID from Data Stream
	switch(keyState)
	{
		case DEC_KEY_BYOK:
			log_info("Using a User-Provided Decryption Key.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], AES_KEY_SIZE);
			// ToDo Generate a Key ID & Store the provided Key in the Key Manager
			break;

		case DEC_KEY_DABA:
			log_info("Searching for a stored Key in the Key Manager.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], KEYID_LEN);
			// ToDo Search Key Manager for a match using Key ID
			break;

		default:
			log_warn("Key State field not Recognized.");
			break;
	}
	// Copy the IV from the Data Stream
	switch(ivState)
	{
		case DEC_IV_BYIV:
			log_info("Using a User-Provided Initialization Vector.");
			memcpy(ivData, &request->inputData[IV_DATA_POS], AES_IV_SIZE);
			break;

		case DEC_IV_DABA:
			log_info("Generating a Random Initialization Vector");
			memcpy(ivData, &request->inputData[IV_DATA_POS], AES_IV_SIZE);
			break;

		default:
			log_warn("IV State field not Recognized.");
			break;
	}
	// Execute the Encryption operation
	codec_result = CryptoEngine_Codec(plaintextData,
										ciphertextLen,
										&request->inputData[PLAINTEXT_POS],
										ciphertextLen,
										ivData,
										keyData);
	// Check if Encryption operation was success
	if(codec_result == CODEC_FAILURE)
	{
		log_error("Decryption operation failed.");
		return OPERATION_DECRYPTION_FAIL;
	}
	// Set the output size in Response Packet
	response->outputSize = ciphertextLen + KEYID_LEN + AES_IV_SIZE;
	// Copy the Key ID into Output Data Buffer
	memcpy(&response->outputData[OUT_KEYID_POS], keyID, KEYID_LEN);
	// Copy the Ciphertext into Output Data Buffer
	memcpy(&response->outputData[OUT_CT_POS], plaintextData, ciphertextLen);
	// Return the OperationStatus value
	return OPERATION_SUCCESS;
}
