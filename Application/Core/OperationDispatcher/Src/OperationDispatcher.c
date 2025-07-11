#include "../../OperationDispatcher/Inc/OperationDispatcher.h"
#include "../../../Handlers/Encryption/Inc/EncryptHandler.h"
#include "../../../Handlers/Decryption/Inc/DecryptHandler.h"
#include "../../../Handlers/Hashing/Inc/HashHandler.h"
#include "../../../Handlers/HMAC/Inc/HMACHandler.h"
#include "../../../Handlers/Random/Inc/RandomHandler.h"
#include "../../../Handlers/OTP/Inc/OTPHandler.h"
#include "../../../Handlers/KeyGen/Inc/KeyGen.h"
#include "../../../Handlers/KeyStore/Inc/KeyStore.h"
#include "../../../Handlers/KeyDelete/Inc/KeyDelete.h"
#include "constants.h"
#include "Logger.h"

OperationStatus_t OperationDispatcher_Dispatch(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    log_info("Dispatching Command.");

    switch (request->cmd)
    {
        case CMD_ENCRYPT:
        	log_info("Calling Encryption Operation.");
            return EncryptHandler_Encrypt(request, response);

        case CMD_DECRYPT:
        	log_info("Calling Decryption Operation.");
            return DecryptHandler_Decrypt(request, response);

        case CMD_HASH:
        	log_info("Calling Hashing Operation.");
            return HashingHandler_Handle(request, response);

        case CMD_HMAC:
        	log_info("Calling HMAC Operation.");
            return HMACHandler_Handle(request, response);

        case CMD_RANDOM:
        	log_info("Calling RNG Operation.");
            return RandomHandler_Handle(request, response);

        case CMD_OTP:
            log_info("Calling OTP Operation.");
            return OTPHandler_Handle(request, response);

        case CMD_KEY_GEN:
        	log_info("Calling Key Generator Operation.");
            return KeyHandler_Generate(request, response);

        case CMD_KEY_STORE:
        	log_info("Calling Key Store Operation.");
            return KeyHandler_Store(request, response);

        case CMD_KEY_DELETE:
        	log_info("Calling Key Delete Operation.");
            return KeyHandler_Delete(request, response);

        case CMD_PING:
        	log_info("Calling Ping Operation.");
        	log_warn("Ping Operation not supported");
            return OPERATION_INVALID_CMD;

        default:
            log_error("Invalid Command code received");
            log_error("Returning Failure Status code");
            return OPERATION_INVALID_CMD;
    }
}
