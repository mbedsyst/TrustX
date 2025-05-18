#include "../../PacketParser/Inc/PacketParser.h"
#include "constants.h"
#include "Logger.h"
#include <string.h>

ParseStatus_t PacketParser_Parse(const uint8_t* rawData, uint16_t rawLength, ParsedPacket_t* parsedPacket)
{
	log_debug("Checking if Data Stream or Packet Structure is NULL.");
	// Check if Data Stream or Packet structure is NULL
	if (!rawData || !parsedPacket)
    {
		log_error("Data Stream or Packet Structure is NULL");
    	return PARSE_NULL_POINTER;
    }
	log_debug("Checking if Data Stream contains less than 12 bytes.");
    // Check if Data Stream contains Data of valid length
    if (rawLength < 12)
    {
    	log_error("Data stream contain less than 12 bytes");
    	return PARSE_INVALID_LENGTH;
    }
    // Set Index to start of Data Stream
    uint16_t index = 0;
    log_debug("Filling up Transaction ID.");
    // Retrieve Transaction ID from Data Stream
    parsedPacket->transactionID = (rawData[index]     << 24) |
                                  (rawData[index + 1] << 16) |
                                  (rawData[index + 2] << 8)  |
                                  (rawData[index + 3]);
    // Set Index to point CMD byte
    index += 4;
    log_debug("Filling up CMD, OPTION and INPUT_SIZE fields.");
    // Retrieve CMD byte from Data Stream
    parsedPacket->cmd = rawData[index++];
    // Retrieve OPTION byte from Data Stream
    parsedPacket->option = rawData[index++];
    // Retrieve INPUT_SIZE half-word from Data Stream
    parsedPacket->inputSize = (rawData[index] << 8) | rawData[index + 1];
    // Set Index to point INPUT_DATA
    index += 2;
    log_debug("Checking if INPUT_SIZE is within the limits.");
    // Check if INPUT_SIZE is under the prescribed limit
    if (parsedPacket->inputSize > MAX_INPUT_DATA_SIZE || (index + parsedPacket->inputSize + 4) > rawLength)
    {
    	log_error("Input Data Size is too big.");
        return PARSE_INPUT_TOO_LARGE;
    }
    log_debug("Copying INPUT DATA to Packet Structure.");
    // Copy INPUT_DATA array from Data Stream to Packet structure
    memcpy(parsedPacket->inputData, &rawData[index], parsedPacket->inputSize);
    // Set Index to point EOD Flag
    index += parsedPacket->inputSize;
    log_debug("Checking the Received EOD word.");
    // Retrieve EOD Flag from Data Stream
    uint32_t receivedEOD = (rawData[index]     << 24) |
                           (rawData[index + 1] << 16) |
                           (rawData[index + 2] << 8)  |
                           (rawData[index + 3]);
    // Check if EOD Flag is valid
    if (receivedEOD != EOD_FLAG)
    {
    	log_error("Received EOD Flag is Invalid");
        return PARSE_EOD_MISMATCH;
    }
    // Return to Indicate successful parsing
    return PARSE_SUCCESS;
}



