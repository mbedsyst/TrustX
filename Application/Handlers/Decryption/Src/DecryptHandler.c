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
	/* ToDo
	 * 1. Parse out Decryption Key State from INPUT DATA
	 * 2. Parse out the Key Data field from INPUT DATA.
	 * 3. Parse out IV State from INPUT DATA
	 * 4. Parse out the IV Data field from INPUT DATA
	 * 5. If Key is provided, store it into an array.
	 * 6. If Key ID is provided, search for the key from the flash, decrypt and store it in an array.
	 * 7. Store the provided IV into an array
	 * 8. Initialize the wolfSSL Decryption context.
	 * 9. Execute the Decryption operation.
	 * 10. Store the used Key into the Flash and increase the usage counter.
	 * 11. Append the Plaintext to the end of the Key ID, as OUTPUT DATA
	 * 12. Assign the size of OUTPUT DATA.
	 * 13. Return the Response Packet
	 * */

	log_info("Handling Decryption operation.");
	// Check if either Request or Response Packet is NULL
	if (!request )
	{
		return OPERATION_INVALID_DATA;
	}
	int codec_result = 0;
	uint8_t keyState, ivState;
	uint32_t keyID;
	static uint8_t keyData[AES_KEY_SIZE], ivData[AES_IV_SIZE];

	keyState = request->inputData[KEY_STATE_POS];
	ivState = request->inputData[IV_STATE_POS];

	const uint16_t ciphertextLen = (request->inputSize) - AES_KEY_SIZE - AES_IV_SIZE - 2;
	static uint8_t plaintextData[ciphertextLen];

	switch(keyState)
	{
		case DEC_KEY_BYOK:
			log_info("Using a User-Provided Decryption Key.");
			memcpy(keyData, &request->inputData[KEY_DATA_POS], AES_KEY_SIZE);
			break;

		case DEC_KEY_DABA:
			log_info("Searching for a stored Key in the Key Manager.");
			// ToDo Search Key Manager for a match using Key ID
			break;

		default:
			log_warn("Key State field not Recognized.");
			break;
	}

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

	codec_result = CryptoEngine_Codec(plaintextData,
										ciphertextLen,
										&request->inputData[PLAINTEXT_POS],
										ciphertextLen,
										ivData,
										keyData);

	if(codec_result == CODEC_FAILURE)
	{
		log_err("Decryption operation failed.");
		return OPERATION_DECRYPTION_FAIL;
	}

	response->outputSize = ciphertextLen + KEYID_LEN + AES_IV_SIZE;
	memcpy(&response->outputData[OUT_KEYID_POS], keyID, KEYID_LEN);
	memcpy(&response->outputData[OUT_CT_POS], ciphertextData, plaintextLen);

	return OPERATION_SUCCESS;
}
