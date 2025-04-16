#ifndef PROTOCOL_PACKETBUILDER_INC_PACKETBUILDER_H_
#define PROTOCOL_PACKETBUILDER_INC_PACKETBUILDER_H_

#include <stdint.h>
#include <stdbool.h>

// Upper limit on valid OUTPUT_DATA in Data Stream
#define MAX_OUTPUT_DATA_SIZE 4096

// Structure to hold outgoing Data Stream
typedef struct
{
    uint32_t transactionID;
    uint16_t outputSize;
    uint8_t outputData[MAX_OUTPUT_DATA_SIZE];
} ResponsePacket_t;

// Enumeration to hold Build Operation status
typedef enum
{
    BUILD_SUCCESS = 0,
    BUILD_NULL_POINTER,
    BUILD_OUTPUT_TOO_LARGE
} BuildStatus_t;

// Function to Build Data Stream from Packet contents
BuildStatus_t PacketBuilder_Build(const ResponsePacket_t* responsePacket, uint8_t* outBuffer, uint16_t* outLength);

#endif /* PROTOCOL_PACKETBUILDER_INC_PACKETBUILDER_H_ */
