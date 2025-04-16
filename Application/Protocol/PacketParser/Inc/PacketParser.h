#ifndef PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_
#define PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_

#include <stdint.h>
#include <stdbool.h>

// Upper limit on valid INPUT_DATA in Data Stream
#define MAX_INPUT_DATA_SIZE 	4096

// Structure to hold incoming Data Stream
typedef struct
{
    uint32_t transactionID;
    uint8_t cmd;
    uint8_t option;
    uint16_t inputSize;
    uint8_t inputData[MAX_INPUT_DATA_SIZE];
} ParsedPacket_t;

// Enumeration to hold Parse Operation status
typedef enum
{
    PARSE_SUCCESS = 0,
    PARSE_INVALID_LENGTH,
    PARSE_EOD_MISMATCH,
    PARSE_NULL_POINTER,
    PARSE_INPUT_TOO_LARGE
} ParseStatus_t;

// Function to Parse contents from Data Stream to fill Packet Structure
ParseStatus_t PacketParser_Parse(const uint8_t* rawData, uint16_t rawLength, ParsedPacket_t* parsedPacket);

#endif /* PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_ */
