#ifndef PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_
#define PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_

#include <stdint.h>
#include <tinycrypt/aes.h>

/**
 * @brief Stateless AES-CTR wrapper over TinyCrypt's tc_ctr_mode.
 *
 * @param out      Output buffer (ciphertext or plaintext)
 * @param outlen   Length of output buffer (must match inlen)
 * @param in       Input buffer (plaintext or ciphertext)
 * @param inlen    Length of input buffer
 * @param ctr      16-byte counter (IV/nonce), NOT modified
 * @param key      16-byte AES-128 key
 * @return int     TC_CRYPTO_SUCCESS (1) or TC_CRYPTO_FAIL (0)
 */
int CryptoEngine_Codec(uint8_t *out, unsigned int outlen,
                            const uint8_t *in, unsigned int inlen,
                            const uint8_t *ctr,
                            const uint8_t *key);

#endif /* PROTOCOL_CRYPTOENGINE_INC_CRYPTOENGINE_H_ */
