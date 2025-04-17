#ifndef HANDLERS_RANDOM_INC_RANDOMHANDLER_H_
#define HANDLERS_RANDOM_INC_RANDOMHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t RandomHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_RANDOM_INC_RANDOMHANDLER_H_ */
