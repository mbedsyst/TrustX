#ifndef PROTOCOL_KEYDERIVATOR_INC_KEYDERIVATOR_H_
#define PROTOCOL_KEYDERIVATOR_INC_KEYDERIVATOR_H_

#include <stdint.h>
#include <stddef.h>

typedef enum
{
	KEY_DERIVATOR_OK = 0,
	KEY_DERIVATOR_NULL_INPUT,
	KEY_DERIVATOR_HMAC_FAIL
}KeyDerivatorStatus_t;

/**
 * Derives a 128-bit AES key using HKDF with SHA256 (HMAC).
 *
 * @param ikm       Pointer to the 20-byte random value.
 * @param uid       Pointer to the 96-bit STM32 Unique ID.
 * @param out_key   Pointer to 16-byte output buffer to hold the AES key.
 * @return          0 on success, non-zero on error.
 */
KeyDerivatorStatus_t HKDF_DeriveKey(const uint8_t *ikm, const uint8_t *uid, uint8_t *out_key);

#endif /* PROTOCOL_KEYDERIVATOR_INC_KEYDERIVATOR_H_ */
