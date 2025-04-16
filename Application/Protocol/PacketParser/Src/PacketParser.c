#include "../../PacketParser/Inc/PacketParser.h"
#include "constants.h"
#include <string.h>

ParseStatus_t PacketParser_Parse(const uint8_t* rawData, uint16_t rawLength, ParsedPacket_t* parsedPacket)
{
	// Check if Data Stream or Packet structure is NULL
    if (!rawData || !parsedPacket)
    {
    	return PARSE_NULL_POINTER;
    }
    // Check if Data Stream contains Data of valid length
    if (rawLength < 12)
    {
    	return PARSE_INVALID_LENGTH;
    }
    // Set Index to start of Data Stream
    uint16_t index = 0;
    // Retrieve Transaction ID from Data Stream
    parsedPacket->transactionID = (rawData[index]     << 24) |
                                  (rawData[index + 1] << 16) |
                                  (rawData[index + 2] << 8)  |
                                  (rawData[index + 3]);
    // Set Index to point CMD byte
    index += 4;
    // Retrieve CMD byte from Data Stream
    parsedPacket->cmd = rawData[index++];
    // Retrieve OPTION byte from Data Stream
    parsedPacket->option = rawData[index++];
    // Retrieve INPUT_SIZE half-word from Data Stream
    parsedPacket->inputSize = (rawData[index] << 8) | rawData[index + 1];
    // Set Index to point INPUT_DATA
    index += 2;
    // Check if INPUT_SIZE is under the prescribed limit
    if (parsedPacket->inputSize > MAX_INPUT_DATA_SIZE || (index + parsedPacket->inputSize + 4) > rawLength)
    {
        return PARSE_INPUT_TOO_LARGE;
    }
    // Copy INPUT_DATA array from Data Stream to Packet structure
    memcpy(parsedPacket->inputData, &rawData[index], parsedPacket->inputSize);
    // Set Index to point EOD Flag
    index += parsedPacket->inputSize;
    // Retrieve EOD Flag from Data Stream
    uint32_t receivedEOD = (rawData[index]     << 24) |
                           (rawData[index + 1] << 16) |
                           (rawData[index + 2] << 8)  |
                           (rawData[index + 3]);
    // Check if EOD Flag is valid
    if (receivedEOD != EOD_FLAG_VALUE)
    {
        return PARSE_EOD_MISMATCH;
    }
    // Return to Indicate successful parsing
    return PARSE_SUCCESS;
}



