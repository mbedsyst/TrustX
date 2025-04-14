#ifndef PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_
#define PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_INPUT_DATA_SIZE 	4096
#define EOD_FLAG_VALUE 			0xDEADBEEF

typedef struct
{
    uint32_t transactionID;
    uint8_t cmd;
    uint8_t option;
    uint16_t inputSize;
    uint8_t inputData[MAX_INPUT_DATA_SIZE];
} ParsedPacket_t;

typedef enum
{
    PARSE_SUCCESS = 0,
    PARSE_INVALID_LENGTH,
    PARSE_EOD_MISMATCH,
    PARSE_NULL_POINTER,
    PARSE_INPUT_TOO_LARGE
} ParseStatus_t;

ParseStatus_t PacketParser_Parse(const uint8_t* rawData, uint16_t rawLength, ParsedPacket_t* parsedPacket);


#endif /* PROTOCOL_PACKETPARSER_INC_PACKETPARSER_H_ */
