#include "KeyDerivator.h"
#include "Generator.h"
#include "Logger.h"
#include <string.h>

extern HASH_HandleTypeDef hhash;

#define HASH_OUTPUT_SIZE 	32  // SHA-256 digest size
#define DERIVED_KEY_SIZE 	16  // 128-bit AES key
#define UID_SIZE         	12  // 96-bit STM32 Unique ID
#define IKM_SIZE         	20  // Random entropy size

KeyDerivatorStatus_t HKDF_DeriveKey(const uint8_t *ikm, const uint8_t *uid, uint8_t *out_key)
{
	//Checking if Input arguments are NULL
    if (!ikm || !uid || !out_key)
    {
        log_error("Key derivation: NULL input");
        return KEY_DERIVATOR_NULL_INPUT;
    }

    uint8_t prk[HASH_OUTPUT_SIZE];
    uint8_t salt[HASH_OUTPUT_SIZE] = {0};  // optional, use zero as per HKDF standard

    // HKDF Extract: PRK = HMAC(salt, IKM)
    if (GenerateHMAC((uint8_t *)ikm, IKM_SIZE, salt, prk) != 0)
    {
        log_error("HKDF Extract failed.");
        return KEY_DERIVATOR_HMAC_FAIL;
    }

    // HKDF Expand: OKM = HMAC(PRK, info | 0x01)
    uint8_t info[UID_SIZE + 1];
    memcpy(info, uid, UID_SIZE);
    info[UID_SIZE] = 0x01;  // Counter = 1

    uint8_t okm[HASH_OUTPUT_SIZE];  // full 32-byte output
    if (GenerateHMAC(info, sizeof(info), prk, okm) != 0)
    {
        log_error("HKDF Expand failed.");
        return KEY_DERIVATOR_HMAC_FAIL;
    }

    // Copy first 16 bytes for AES-128
    memcpy(out_key, okm, DERIVED_KEY_SIZE);
    return KEY_DERIVATOR_OK;
}
