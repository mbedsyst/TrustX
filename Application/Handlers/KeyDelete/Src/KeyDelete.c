#include "../../KeyDelete/Inc/KeyDelete.h"

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


OperationStatus_t KeyHandler_Delete(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    log_info("Handling Key Delete operation.");

    // Check if either Request or Response Packet is NULL
    if (!request)
    {
    	log_error("Request Packet is NULL.");
        return OPERATION_INVALID_INPUT_DATA;
    }

    // Declare variables to hold Key ID.
    uint8_t keyID[4] = {0};
    uint32_t keyID_32 = 0;

    // Retrieve Key ID from Request Packet
    memcpy(keyID, &request->inputData[0], 4);

    // Convert 4-byte Key ID array into 32-bit unsigned integer
    keyID_32 = ConvertKeyIDToUint32(keyID);
    // Call Key Manager to Add Key to Storage medium
    if(KeyManager_DeleteKey(keyID_32) != 0)
    {
    	log_error("Failed to Delete Key from Storage Medium.");
    	return OPERATION_KEY_DELETE_FAIL;
    }

    // Set the Output Size to 2 for zeroing output data
    response->outputSize = 2;
    memset(&response->outputData, 0, 2);
    log_info("Key Delete Operation complete.");
    return OPERATION_SUCCESS;
}
