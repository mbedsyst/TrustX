#include "../../HSMManager/Inc/HSMManager.h"
#include "Logger.h"

void HSMManager_Init(void)
{
    Logger_Log("HSM Initialized.");
    // Initialize any internal state, EEPROM, FLASH, crypto libraries, etc.
}

OperationStatus_t HSMManager_ProcessCommand(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    Logger_Log("Processing new HSM command...");

    // Validate packet contents, do CRC checks if needed
    // Pass to dispatcher
    return OperationDispatcher_Dispatch(request, response);
}
