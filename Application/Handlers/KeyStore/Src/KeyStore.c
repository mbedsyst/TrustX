#include "../../KeyStore/Inc/KeyStore.h"

#include "KeyManager.h"
#include "Generator.h"
#include "Logger.h"

#include "constants.h"
#include "types.h"

#include <stdlib.h>
#include <string.h>

#include "stm32h5xx_hal.h"

static uint32_t ConvertKeyIDToUint32(const uint8_t keyID[4])
{
    return ((uint32_t)keyID[0]) |
           ((uint32_t)keyID[1] << 8) |
           ((uint32_t)keyID[2] << 16) |
           ((uint32_t)keyID[3] << 24);
}

OperationStatus_t KeyHandler_Store(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    log_info("Handling Key Store operation.");

    // Check if either Request or Response Packet is NULL
    if (!request )
    {
    	log_error("Request Packet is NULL.");
        return OPERATION_INVALID_INPUT_DATA;
    }

    // Declare variables to hold Key ID, Key Value, and Key Usage information.
    uint8_t keyID[4] = {0};
    uint32_t keyID_32 = 0;
    uint8_t keyVal[32] = {0};

    // Retrieve Key Size, Key Usage and Key Value from Request Packet
    uint8_t keySize = request->inputSize - 1;
    uint8_t keyUsage = request->inputData[0];
    memcpy(keyVal, &request->inputData[1], keySize);

    // Generate 4-byte Key ID
    GenerateKEYID(keyID);
    // Convert 4-byte Key ID array into 32-bit unsigned integer
    keyID_32 = ConvertKeyIDToUint32(keyID);
    // Call Key Manager to Add Key to Storage medium
    if(KeyManager_AddKey(keyID_32, keyVal, keySize, KEY_ORIGIN_PROVIDED, keyUsage) != 0)
    {
    	log_error("Failed to Add Key to Storage Medium.");
    	return OPERATION_KEY_STORE_FAIL;
    }

    // Set the Output Size to the size of Key ID
    response->outputSize = 4;
    memcpy(&response->outputData, keyID, 4);
    log_info("Key Store Operation complete.");
    return OPERATION_SUCCESS;
}
