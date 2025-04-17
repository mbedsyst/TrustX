#include "../../PacketBuilder/Inc/PacketBuilder.h"
#include "constants.h"
#include <string.h>


BuildStatus_t PacketBuilder_Build(const ResponsePacket_t* responsePacket, uint8_t* outBuffer, uint16_t* outLength)
{
	// Check if Response, Output and Output Size are NULL
    if (!responsePacket || !outBuffer || !outLength)
    {
    	return BUILD_NULL_POINTER;
    }
    // Check if Output Size is under the specified limits
    if (responsePacket->outputSize > MAX_OUTPUT_DATA_SIZE)
    {
    	return BUILD_OUTPUT_TOO_LARGE;
    }
    // Set Index to start of Data Stream
    uint16_t index = 0;
    // Fill Data Stream with the corresponding Transaction ID
    outBuffer[index++] = (responsePacket->transactionID >> 24) & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID >> 16) & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID >> 8)  & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID)       & 0xFF;
    // Fill Data Stream with the Output Data Size
    outBuffer[index++] = (responsePacket->outputSize >> 8) & 0xFF;
    outBuffer[index++] = (responsePacket->outputSize)      & 0xFF;
    // Copy Output Data contents to Data Stream
    memcpy(&outBuffer[index], responsePacket->outputData, responsePacket->outputSize);
    // Set Index to point to EOD Flag
    index += responsePacket->outputSize;
    // Add EOD Flag to Data Stream
    outBuffer[index++] = (EOD_FLAG >> 24) & 0xFF;
    outBuffer[index++] = (EOD_FLAG >> 16) & 0xFF;
    outBuffer[index++] = (EOD_FLAG >> 8)  & 0xFF;
    outBuffer[index++] = (EOD_FLAG)       & 0xFF;
    // Set Index to point to End of Data Stream
    *outLength = index;
    // Return to indicate successful Packet Build
    return BUILD_SUCCESS;
}
