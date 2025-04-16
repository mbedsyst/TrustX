#include "../../OperationDispatcher/Inc/OperationDispatcher.h"
#include "../../../Handlers/Encryption/Inc/EncryptHandler.h"
#include "../../../Handlers/Decryption/Inc/DecryptHandler.h"
#include "../../../Handlers/Hashing/Inc/HashHandler.h"
#include "../../../Handlers/Keys/Inc/KeyHandler.h"
#include "../../../Handlers/Random/Inc/RandomHandler.h"
#include "constants.h"
#include "Logger.h"

OperationStatus_t OperationDispatcher_Dispatch(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    log_info("Dispatching command...");

    switch (request->cmd)
    {
        case CMD_ENCRYPT:
            return EncryptHandler_Handle(request, response);

        case CMD_DECRYPT:
            return DecryptHandler_Handle(request, response);

        case CMD_HASH:
            return HashHandler_Handle(request, response);

        case CMD_RANDOM:
            return RandomHandler_Handle(request, response);

        case CMD_KEY_STORE:
        case CMD_KEY_RETRIEVE:
        case CMD_KEY_ERASE:
            return KeyHandler_Handle(request, response);

        case CMD_PING:
            response->status = RESPONSE_OK;
            response->dataLength = 4;
            response->data[0] = 'P';
            response->data[1] = 'O';
            response->data[2] = 'N';
            response->data[3] = 'G';
            return OPERATION_SUCCESS;

        default:
            Logger_Log("Invalid command.");
            return OPERATION_INVALID_CMD;
    }
}
