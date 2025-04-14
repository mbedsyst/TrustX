#ifndef CORE_OPERATIONDISPATCHER_INC_OPERATIONDISPATCHER_H_
#define CORE_OPERATIONDISPATCHER_INC_OPERATIONDISPATCHER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t OperationDispatcher_Dispatch(const ParsedPacket_t* request, ResponsePacket_t* response);


#endif /* CORE_OPERATIONDISPATCHER_INC_OPERATIONDISPATCHER_H_ */
