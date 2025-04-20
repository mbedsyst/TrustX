#include "../../Hashing/Inc/HashHandler.h"
#include <string.h>

static OperationStatus_t compute_sha224(const uint8_t* input, uint16_t length, uint8_t* output);
static OperationStatus_t compute_sha256(const uint8_t* input, uint16_t length, uint8_t* output);
static OperationStatus_t compute_hmac(const uint8_t* input, uint16_t length, const mbedtls_md_type_t md_type, uint8_t* output);

OperationStatus_t HashingHandler_Handle(const ParsedPacket_t* request, ResponsePacket_t* response)
{
    if (!request || !response)
        return OPERATION_NULL_POINTER;

    OperationStatus_t status = OPERATION_INVALID_OPTION;
    uint8_t digest[32]; // Max digest size for SHA256

    switch (request->option)
    {
        case OPTION_HASH_SHA224:
            status = compute_sha224(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HASH_SHA256:
            status = compute_sha256(request->inputData, request->inputSize, digest);
            break;

        case OPTION_HMAC_SHA224:
            status = compute_hmac(request->inputData, request->inputSize, MBEDTLS_MD_SHA224, digest);
            break;

        case OPTION_HMAC_SHA256:
            status = compute_hmac(request->inputData, request->inputSize, MBEDTLS_MD_SHA256, digest);
            break;

        default:
            return OPERATION_INVALID_OPTION;
    }

    if (status != OPERATION_SUCCESS)
        return status;

    // Fill in the response
    response->transactionID = request->transactionID;
    response->status = 0x00; // Success status
    response->outputSize = (request->option == OPTION_HASH_SHA224 || request->option == OPTION_HMAC_SHA224) ? 28 : 32;
    memcpy(response->outputData, digest, response->outputSize);

    return OPERATION_SUCCESS;
}

static OperationStatus_t compute_sha224(const uint8_t* input, uint16_t length, uint8_t* output)
{
    if (mbedtls_sha224_ret(input, length, output, 0) != 0)
        return OPERATION_HASHING_FAILED;
    return OPERATION_SUCCESS;
}

static OperationStatus_t compute_sha256(const uint8_t* input, uint16_t length, uint8_t* output)
{
    if (mbedtls_sha256_ret(input, length, output, 0) != 0)
        return OPERATION_HASHING_FAILED;
    return OPERATION_SUCCESS;
}

static OperationStatus_t compute_hmac(const uint8_t* input, uint16_t length, const mbedtls_md_type_t md_type, uint8_t* output)
{
    // Dummy HMAC key for now
    static const uint8_t hmac_key[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
        0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC,
        0xDD, 0xEE, 0xFF, 0x00
    };
    const size_t key_len = sizeof(hmac_key);

    const mbedtls_md_info_t* md_info = mbedtls_md_info_from_type(md_type);
    if (!md_info)
        return OPERATION_HASHING_FAILED;

    if (mbedtls_md_hmac(md_info, hmac_key, key_len, input, length, output) != 0)
        return OPERATION_HASHING_FAILED;

    return OPERATION_SUCCESS;
}

