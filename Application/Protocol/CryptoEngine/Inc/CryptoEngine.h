#ifndef PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_
#define PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Encrypt or decrypt using AES-128 in CTR mode.
 *
 * @param[out] out      Output buffer (ciphertext or plaintext).
 * @param[in]  outlen   Length of the output buffer (in bytes).
 * @param[in]  in       Input buffer (plaintext or ciphertext).
 * @param[in]  inlen    Length of the input buffer (in bytes).
 * @param[in]  ctr      Initialization vector / counter (16 bytes).
 * @param[in]  ctrlen   Length of ctr buffer (must be 16).
 * @param[in]  key      AES-128 key (16 bytes).
 * @param[in]  keylen   Length of key (must be 16).
 *
 * @return 0 on failure, 1 on success (TC_CRYPTO_SUCCESS).
 */
int hsm_ctr_encrypt(uint8_t *out, unsigned int outlen,
                    const uint8_t *in, unsigned int inlen,
                    const uint8_t *ctr, unsigned int ctrlen,
                    const uint8_t *key, unsigned int keylen);

#define hsm_ctr_decrypt hsm_ctr_encrypt

#ifdef __cplusplus
}
#endif

#endif /* PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_ */
