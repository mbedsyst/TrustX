#ifndef PROTOCOL_KEYMANAGER_INC_KEYMANAGER_H_
#define PROTOCOL_KEYMANAGER_INC_KEYMANAGER_H_

#include <stdint.h>

#define MAX_KEY_SIZE     32
#define MAX_USAGE_COUNT  100

typedef enum
{
    KEY_ORIGIN_PROVIDED = 0,
    KEY_ORIGIN_GENERATED = 1
} KeyOrigin_t;

typedef enum
{
    USAGE_ENCRYPT = 0x01,
    USAGE_DECRYPT = 0x02,
    USAGE_HMAC    = 0x04
} KeyUsage_t;

typedef enum
{
    SIZE_128 = 16,
    SIZE_192 = 24,
    SIZE_256 = 32
} KeySize_t;

typedef enum
{
    KM_STATUS_OK = 0,              // Operation completed successfully
    KM_STATUS_ERROR,               // General failure
    KM_STATUS_INVALID_OP,          // Invalid or unsupported operation
	KM_STATUS_NO_HEAP,			   // No more space in Heap to allocate for Key Entry
    KM_STATUS_KEY_NOT_FOUND,       // Requested key ID not found
    KM_STATUS_KEY_ALREADY_EXISTS,  // Key ID already exists when trying to add
    KM_STATUS_INVALID_KEY_SIZE,    // Key size not acceptable
    KM_STATUS_USAGE_EXCEEDED,      // Usage count exceeded limit
    KM_STATUS_STORAGE_FULL,        // No more space in flash for new keys
    KM_STATUS_HMAC_MISMATCH,       // HMAC check failed (integrity/auth failure)
    KM_STATUS_INVALID_ARGUMENT,    // Input parameters invalid or NULL
    KM_STATUS_UNALIGNED_ACCESS,    // Flash or memory alignment error
    KM_STATUS_FLASH_ERROR,         // Flash read/write/erase failure
    KM_STATUS_NOT_IMPLEMENTED,      // Operation not supported yet
	KM_STATUS_IV_FAIL,
	KM_STATUS_CODEC_FAIL,
	KM_STATUS_HMAC_FAIL,
	KM_STATUS_WRITE_KEY_FAIL,
	KM_STATUS_READ_KEY_FAIL,
	KM_STATUS_UPDATE_KEY_FAIL,
	KM_STATUS_DELETE_KEY_FAIL
} KeyManagerStatus_t;

typedef struct __attribute__((packed))
{
    uint32_t key_id;                     // Unique ID for the key
    uint8_t  usage_count;                // Remaining uses (max = 100)
    uint8_t  origin;                     // Provided or Generated
    uint8_t  usage;                      // Bitmask: Encrypt, Decrypt, HMAC
    uint8_t  key_size;                   // Key size in bytes
    uint8_t  key_val[MAX_KEY_SIZE];   // Encrypted key data
    uint8_t  iv[16];                     // IV used during encryption
    uint8_t  hmac[32];                   // HMAC-SHA256 truncated (e.g., first 16 bytes)
} KeyEntry_t;

KeyManagerStatus_t KeyManager_Init(void);
KeyManagerStatus_t KeyManager_AddKey(uint32_t keyID, uint8_t* key, uint8_t keySize, uint8_t keyOrigin, uint8_t keyUsage);
KeyManagerStatus_t KeyManager_GetKey(uint32_t keyID, uint8_t* key);
KeyManagerStatus_t KeyManager_DeleteKey(uint32_t keyID);


#endif /* PROTOCOL_KEYMANAGER_INC_KEYMANAGER_H_ */
