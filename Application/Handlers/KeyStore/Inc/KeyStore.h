#ifndef HANDLERS_KEYSTORE_INC_KEYSTORE_H_
#define HANDLERS_KEYSTORE_INC_KEYSTORE_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t KeyHandler_Store(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_KEYSTORE_INC_KEYSTORE_H_ */
