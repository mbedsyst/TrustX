#ifndef HANDLERS_KEYS_INC_KEYHANDLER_H_
#define HANDLERS_KEYS_INC_KEYHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t KeyHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_KEYS_INC_KEYHANDLER_H_ */
