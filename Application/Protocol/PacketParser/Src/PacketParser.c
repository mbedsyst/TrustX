#include "../../PacketParser/Inc/PacketParser.h"
#include <string.h>

ParseStatus_t PacketParser_Parse(const uint8_t* rawData, uint16_t rawLength, ParsedPacket_t* parsedPacket)
{
    if (!rawData || !parsedPacket) return PARSE_NULL_POINTER;
    if (rawLength < 12) return PARSE_INVALID_LENGTH; // Minimum packet size

    // Extract fields
    uint16_t index = 0;

    parsedPacket->transactionID = (rawData[index]     << 24) |
                                  (rawData[index + 1] << 16) |
                                  (rawData[index + 2] << 8)  |
                                  (rawData[index + 3]);
    index += 4;

    parsedPacket->cmd = rawData[index++];
    parsedPacket->option = rawData[index++];

    parsedPacket->inputSize = (rawData[index] << 8) | rawData[index + 1];
    index += 2;

    if (parsedPacket->inputSize > MAX_INPUT_DATA_SIZE || (index + parsedPacket->inputSize + 4) > rawLength)
    {
        return PARSE_INPUT_TOO_LARGE;
    }

    memcpy(parsedPacket->inputData, &rawData[index], parsedPacket->inputSize);
    index += parsedPacket->inputSize;

    uint32_t receivedEOD = (rawData[index]     << 24) |
                           (rawData[index + 1] << 16) |
                           (rawData[index + 2] << 8)  |
                           (rawData[index + 3]);

    if (receivedEOD != EOD_FLAG_VALUE) {
        return PARSE_EOD_MISMATCH;
    }

    return PARSE_SUCCESS;
}



