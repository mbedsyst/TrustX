#include "CryptoEngine.h"
#include <tinycrypt/constants.h>
#include <tinycrypt/ctr_mode.h>
#include <string.h>

int CryptoEngine_Codec(uint8_t *out, unsigned int outlen,
                            const uint8_t *in, unsigned int inlen,
                            const uint8_t *ctr,
                            const uint8_t *key)
{
    if (!out || !in || !ctr || !key || outlen != inlen || inlen == 0)
    {
        return TC_CRYPTO_FAIL;
    }

    struct tc_aes_key_sched_struct sched;
    if (tc_aes128_set_encrypt_key(&sched, key) != TC_CRYPTO_SUCCESS)
    {
        return TC_CRYPTO_FAIL;
    }

    uint8_t ctr_copy[TC_AES_BLOCK_SIZE];
    memcpy(ctr_copy, ctr, TC_AES_BLOCK_SIZE);

    int result = tc_ctr_mode(out, outlen, in, inlen, ctr_copy, &sched);

    return result;
}
