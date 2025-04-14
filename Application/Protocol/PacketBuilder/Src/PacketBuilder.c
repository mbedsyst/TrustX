#include "../../PacketBuilder/Inc/PacketBuilder.h"
#include <string.h>

BuildStatus_t PacketBuilder_Build(const ResponsePacket_t* responsePacket, uint8_t* outBuffer, uint16_t* outLength)
{
    if (!responsePacket || !outBuffer || !outLength) return BUILD_NULL_POINTER;
    if (responsePacket->outputSize > MAX_OUTPUT_DATA_SIZE) return BUILD_OUTPUT_TOO_LARGE;

    uint16_t index = 0;

    // Transaction ID
    outBuffer[index++] = (responsePacket->transactionID >> 24) & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID >> 16) & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID >> 8)  & 0xFF;
    outBuffer[index++] = (responsePacket->transactionID)       & 0xFF;

    // Output Size
    outBuffer[index++] = (responsePacket->outputSize >> 8) & 0xFF;
    outBuffer[index++] = (responsePacket->outputSize)      & 0xFF;

    // Output Data
    memcpy(&outBuffer[index], responsePacket->outputData, responsePacket->outputSize);
    index += responsePacket->outputSize;

    // EOD Flag
    outBuffer[index++] = (EOD_FLAG_VALUE >> 24) & 0xFF;
    outBuffer[index++] = (EOD_FLAG_VALUE >> 16) & 0xFF;
    outBuffer[index++] = (EOD_FLAG_VALUE >> 8)  & 0xFF;
    outBuffer[index++] = (EOD_FLAG_VALUE)       & 0xFF;

    *outLength = index;

    return BUILD_SUCCESS;
}
