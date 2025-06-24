#ifndef HANDLERS_KEYGEN_INC_KEYGEN_H_
#define HANDLERS_KEYGEN_INC_KEYGEN_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t KeyHandler_Generate(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_KEYGEN_INC_KEYGEN_H_ */
