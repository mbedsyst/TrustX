#include "CryptoEngine.h"
#include <tinycrypt/ctr_mode.h>
#include <tinycrypt/aes.h>
#include <string.h> // Optional, for memcpy if needed

int hsm_ctr_encrypt(uint8_t *out, unsigned int outlen,
                    const uint8_t *in, unsigned int inlen,
                    const uint8_t *ctr, unsigned int ctrlen,
                    const uint8_t *key, unsigned int keylen)
{
    if (!out || !in || !ctr || !key) {
        return 0; //ToDo Status code for returning
    }

    if (keylen != TC_AES_KEY_SIZE || ctrlen != TC_AES_BLOCK_SIZE) {
        return 0; //ToDo Status code for returning
    }

    struct tc_aes_key_sched_struct sched;

    if (tc_aes128_set_encrypt_key(&sched, key) != TC_CRYPTO_SUCCESS) {
        return 0; //ToDo Status code for returning
    }

    int result = tc_ctr_mode(out, outlen, in, inlen, (uint8_t *)ctr, &sched);

    // Optional: clear the key schedule for security
    memset(&sched, 0, sizeof(sched));

    return result;
}

