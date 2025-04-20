#ifndef HANDLERS_HASHING_INC_HASHHANDLER_H_
#define HANDLERS_HASHING_INC_HASHHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t HashingHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_HASHING_INC_HASHHANDLER_H_ */
