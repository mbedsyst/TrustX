#ifndef HANDLERS_ENCRYPTION_INC_ENCRYPTHANDLER_H_
#define HANDLERS_ENCRYPTION_INC_ENCRYPTHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t EncryptHandler_Encrypt(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_ENCRYPTION_INC_ENCRYPTHANDLER_H_ */
