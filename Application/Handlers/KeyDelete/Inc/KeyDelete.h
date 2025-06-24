#ifndef HANDLERS_KEYDELETE_INC_KEYDELETE_H_
#define HANDLERS_KEYDELETE_INC_KEYDELETE_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t KeyHandler_Delete(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_KEYDELETE_INC_KEYDELETE_H_ */
