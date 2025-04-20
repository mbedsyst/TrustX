#include "../../OperationDispatcher/Inc/OperationDispatcher.h"
#include "../../../Handlers/Encryption/Inc/EncryptHandler.h"
#include "../../../Handlers/Decryption/Inc/DecryptHandler.h"
#include "../../../Handlers/Hashing/Inc/HashHandler.h"
#include "../../../Handlers/Keys/Inc/KeyHandler.h"
#include "../../../Handlers/Random/Inc/RandomHandler.h"
#include "constants.h"
#include "types.h"
#include "Logger.h"

OperationStatus_t OperationDispatcher_Dispatch(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    log_info("Dispatching Command.");

    switch (request->cmd)
    {
        case CMD_ENCRYPT:
        	log_info("Calling Encryption Operation.");
        	log_warn("Encryption Operation not supported");
            return OPERATION_UNKNOWN_ERROR;

        case CMD_DECRYPT:
        	log_info("Calling Decryption Operation.");
        	log_warn("Decryption Operation not supported");
            return OPERATION_UNKNOWN_ERROR;

        case CMD_HASH:
        	log_info("Calling Hashing Operation.");
        	log_warn("Hashing Operation not supported");
            return OPERATION_UNKNOWN_ERROR;

        case CMD_RANDOM:
            return RandomHandler_Handle(request, response);

        case CMD_KEY_STORE:
        case CMD_KEY_RETRIEVE:
        case CMD_KEY_ERASE:
        	log_info("Calling Key Management.");
        	log_warn("Key Management Operation not supported");
            return OPERATION_UNKNOWN_ERROR;

        case CMD_PING:
        	log_info("Calling Ping Operation.");
        	log_warn("Ping Operation not supported");
            return OPERATION_UNKNOWN_ERROR;

        default:
            log_error("Invalid Command code received");
            log_error("Returning Failure Status code");
            return OPERATION_INVALID_CMD;
    }
}
