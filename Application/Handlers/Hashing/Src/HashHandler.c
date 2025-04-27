#include "../../Hashing/Inc/HashHandler.h"
#include "constants.h"
#include "types.h"
#include <string.h>

OperationStatus_t HashingHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response)
{

    if (!request || !response)
    {
        return OPERATION_NULL_POINTER;
    }

    OperationStatus_t status = OPERATION_INVALID_OPTION;
    uint8_t digest[64]; // Max digest size for SHA256

    switch (request->option)
    {
        case OPTION_HASH_SHA224:
        	log_info("SHA224 Hashing Algorithm Selected.");
            // status = compute_sha224(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HASH_SHA256:
        	log_info("SHA256 Hashing Algorithm Selected.");
            // status = compute_sha256(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HASH_SHA384:
        	log_info("SHA384 Hashing Algorithm Selected.");
            // status = compute_sha256(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HASH_SHA512:
        	log_info("SHA512 Hashing Algorithm Selected.");
            // status = compute_sha256(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HMAC_SHA224:
        	log_info("SHA224 HMAC Hashing Algorithm Selected.");
            // status = compute_hmac(request->inputData, request->inputSize, MBEDTLS_MD_SHA224, digest);
            break;

        case OPTION_HMAC_SHA256:
        	log_info("SHA256 HMAC Hashing Algorithm Selected.");
            // status = compute_hmac(request->inputData, request->inputSize, MBEDTLS_MD_SHA256, digest);
            break;

        default:
            return OPERATION_INVALID_OPTION;
    }

    if (status != OPERATION_SUCCESS)
        return status;

    // Fill in the response
    response->transactionID = request->transactionID;
    response->outputSize = (request->option == OPTION_HASH_SHA224 || request->option == OPTION_HMAC_SHA224) ? 28 : 32;
    memcpy(response->outputData, digest, response->outputSize);

    return OPERATION_SUCCESS;
}
