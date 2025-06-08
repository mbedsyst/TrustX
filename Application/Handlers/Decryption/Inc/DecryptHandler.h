#ifndef HANDLERS_DECRYPTION_INC_DECRYPTHANDLER_H_
#define HANDLERS_DECRYPTION_INC_DECRYPTHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t DecryptHandler_Decrypt(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif /* HANDLERS_DECRYPTION_INC_DECRYPTHANDLER_H_ */
