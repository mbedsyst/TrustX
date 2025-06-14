#ifndef HANDLERS_HMAC_INC_HMACHANDLER_H_
#define HANDLERS_HMAC_INC_HMACHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t HMACHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_HMAC_INC_HMACHANDLER_H_ */
