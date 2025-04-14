#ifndef CORE_HSMMANAGER_INC_HSMMANAGER_H_
#define CORE_HSMMANAGER_INC_HSMMANAGER_H_

#include "../../constants.h"
#include "PacketParser.h"
#include "PacketBuilder.h"
#include "../../OperationDispatcher/Inc/OperationDispatcher.h"


void HSMManager_Init(void);
OperationStatus_t HSMManager_ProcessCommand(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* CORE_HSMMANAGER_INC_HSMMANAGER_H_ */
