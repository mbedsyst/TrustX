#include "../../Encryption/Inc/EncryptHandler.h"
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
#define OUT_IV_POS		4
#define OUT_CT_POS		20

OperationStatus_t EncryptHandler_Encrypt(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	log_info("Handling Encryption operation.");
	// Check if either Request or Response Packet is NULL
	if (!request )
	{
		return OPERATION_INVALID_DATA;
	}
	int codec_result = 0;
	uint8_t keyState, ivState;
	uint32_t keyID = 0;
	static uint8_t keyData[AES_KEY_SIZE], ivData[AES_IV_SIZE];

	keyState = request->inputData[KEY_STATE_POS];
	ivState = request->inputData[IV_STATE_POS];

	uint16_t plaintextLen = (request->inputSize) - AES_KEY_SIZE - AES_IV_SIZE - 2;
	//static uint8_t ciphertextData[plaintextLen];
	uint8_t *ciphertextData = malloc(plaintextLen);
	if (ciphertextData == NULL)
	{
	    // Handle allocation failure
	}

	switch(keyState)
	{
		case ENC_KEY_BYOK:
			log_info("Using a User-Provided Encryption Key.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], AES_KEY_SIZE);
			break;

		case ENC_KEY_DABA:
			log_info("Searching for a stored Key in the Key Manager.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], KEYID_LEN);
			// ToDo Search Key Manager for a match using Key ID
			break;

		case ENC_KEY_GYOK:
			log_info("Generating a Random Encryption Key.");
			GenerateKEY(keyData);
			break;

		default:
			log_warn("Key State field not Recognized.");
			break;
	}

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

	codec_result = CryptoEngine_Codec(ciphertextData,
										plaintextLen,
										&request->inputData[PLAINTEXT_POS],
										plaintextLen,
										ivData,
										keyData);

	if(codec_result == CODEC_FAILURE)
	{
		log_error("Encryption operation failed.");
		return OPERATION_ENCRYPTION_FAIL;
	}

	response->outputSize = plaintextLen + KEYID_LEN + AES_IV_SIZE;
	memcpy(&response->outputData[OUT_KEYID_POS], (uint8_t *)keyID, KEYID_LEN);
	memcpy(&response->outputData[OUT_IV_POS], ivData, AES_IV_SIZE);
	memcpy(&response->outputData[OUT_CT_POS], ciphertextData, plaintextLen);

	return OPERATION_SUCCESS;
}
