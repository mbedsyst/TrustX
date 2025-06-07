#ifndef HANDLERS_OTP_INC_OTPHANDLER_H_
#define HANDLERS_OTP_INC_OTPHANDLER_H_

#include "types.h"
#include "PacketParser.h"
#include "PacketBuilder.h"

OperationStatus_t OTPHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response);

#endif
