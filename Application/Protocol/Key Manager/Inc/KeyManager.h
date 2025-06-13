#ifndef PROTOCOL_KEY_MANAGER_INC_KEYMANAGER_H_
#define PROTOCOL_KEY_MANAGER_INC_KEYMANAGER_H_

#include <stdint.h>

#define MAX_KEY_SIZE     32
#define MAX_USAGE_COUNT  100

typedef enum
{
    KEY_ORIGIN_PROVIDED = 0,
    KEY_ORIGIN_GENERATED = 1
} key_origin_t;

typedef enum
{
    USAGE_ENCRYPT = 0x01,
    USAGE_DECRYPT = 0x02,
    USAGE_HMAC    = 0x04
} key_usage_t;

typedef struct
{
    uint32_t key_id;                     // Unique ID for the key
    uint8_t  usage_count;                // Remaining uses (max = 100)
    uint8_t  origin;                     // Provided or Generated
    uint8_t  usage;                      // Bitmask: Encrypt, Decrypt, HMAC
    uint8_t  key_size;                   // Key size in bytes
    uint8_t  ciphertext[MAX_KEY_SIZE];   // Encrypted key data
    uint8_t  iv[16];                     // IV used during encryption
    uint8_t  hmac[32];                   // HMAC-SHA256 truncated (e.g., first 16 bytes)
} KeyEntry_t;


typedef enum
{
    KM_OP_NONE = 0,            // No operation
    KM_OP_ADD_KEY,             // Add a new key entry
    KM_OP_DELETE_KEY,          // Delete a key by ID
    KM_OP_UPDATE_KEY,          // Modify an existing key entry
    KM_OP_GET_KEY,             // Retrieve a key by ID
    KM_OP_LIST_KEYS,           // Get metadata for all keys
    KM_OP_INCREMENT_USAGE,     // Decrement/increment usage count
    KM_OP_VALIDATE_KEY,        // Validate key (CRC/hash check)
    KM_OP_RESET_USAGE,         // Reset usage count
    KM_OP_WIPE_ALL_KEYS        // Clear all keys from flash
} KeyManager_Op_t;

KeyManager_Op_t KeyManager_AddKey(uint8_t* keyID, uint8_t* key, uint8_t keySize);
KeyManager_Op_t KeyManager_GetKey(uint8_t* keyID, uint8_t* key);
KeyManager_Op_t KeyManager_DeleteKey(uint8_t keyID);


#endif /* PROTOCOL_KEY_MANAGER_INC_KEYMANAGER_H_ */
