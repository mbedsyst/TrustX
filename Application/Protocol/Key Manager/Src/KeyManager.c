#include "KeyManager.h"
#include "FlashManager.h"
#include "CryptoEngine.h"
#include "Generator.h"
#include "Logger.h"
#include "stdlib.h"
#include "string.h"

#define KEY_ID_POS				0
#define KEY_USECOUNT_POS		4
#define KEY_ORIGIN_POS			5
#define KEY_USAGE_POS			6
#define KEY_SIZE_POS			7
#define KEY_VAL_POS				8
#define KEY_IV_POS				40
#define KEY_HMAC_POS			56

#define KEY_ID_SIZE				4
#define KEY_USECOUNT_SIZE		1
#define KEY_ORIGIN_SIZE			1
#define KEY_USAGE_SIZE			1
#define KEY_SIZE_SIZE			1
#define KEY_VAL_SIZE			32
#define KEY_IV_SIZE				16
#define KEY_HMAC_SIZE			32

#define MAX_SECTORS 			2048
#define ENTRY_HEADER_SIZE 		5 // 1 byte validity + 4 bytes key_id
#define VALIDITY_FLAG_POS		0
#define VALID_ENTRY_FLAG 		0xAA
#define INVALID_ENTRY_FLAG 		0x55

// Lookup Table for Key ID to Sector Mapping
static uint32_t key_lookup_table[MAX_SECTORS];
static uint16_t total_keys = 0;

// Returns sector number if key is found, else -1
static int16_t KeyManager_FindKey(uint32_t key_id)
{
    for (uint16_t sector = 0; sector < MAX_SECTORS; sector++)
    {
        if (key_lookup_table[sector] == key_id)
		{
        	return sector;
		}
    }
    return -1;
}

// Returns the first empty sector
static int16_t KeyManager_FindFreeSector(void)
{
    for (uint16_t sector = 0; sector < MAX_SECTORS; sector++)
    {
        if (key_lookup_table[sector] == 0xFFFFFFFF)
        {
            return sector;
        }
    }
    return -1;
}

// Getter for total number of active keys
uint16_t KeyManager_GetTotalKeys(void)
{
    return total_keys;
}

// Initializes the Key Manager and builds the lookup table
KeyManagerStatus_t KeyManager_Init(void)
{
	// Array to hold the Identifier Header
    uint8_t entry_header[ENTRY_HEADER_SIZE];
    // Zero Initialize the Look-Up Table
    memset(key_lookup_table, 0xFF, sizeof(key_lookup_table)); // 0xFFFFFFFF indicates unused
    // Set Total Key count to Zero
    total_keys = 0;
    // Fill up the Look-Up Table
    for (uint16_t sector = 0; sector < MAX_SECTORS; sector++)
    {
        // Read first 5 bytes of each entry to check for valid entry
        if (FlashManager_ReadIdentifier(sector, entry_header) != FLASH_MANAGER_OK)
        {
            log_error("Failed to read sector %u during KeyManager Init", sector);
            continue;
        }
        // Check the Validity Flag
        if (entry_header[0] == VALID_ENTRY_FLAG)
        {
        	// Retrieve the Key ID from Identifier Header
            uint32_t key_id = (entry_header[1] << 24) |
                              (entry_header[2] << 16) |
                              (entry_header[3] << 8)  |
                              (entry_header[4]);
            // Store the Key ID in the Look-Up Table
            key_lookup_table[sector] = key_id;
            // Increment the Total Key count
            total_keys++;
            log_info("Key ID 0x%08X found at sector %u", key_id, sector);
        }
    }

    log_info("KeyManager Initialization completed with %u keys loaded.", total_keys);
    return KM_STATUS_OK;
}

KeyManagerStatus_t KeyManager_AddKey(uint32_t keyID, uint8_t* key, uint8_t keySize, uint8_t keyOrigin, uint8_t keyUsage)
{
	uint16_t sectorNumber = 0;
	sectorNumber = KeyManager_FindFreeSector();
	if(sectorNumber < 0)
	{
		log_error("No free sectors are available. Key Manager full.");
		return KM_STATUS_STORAGE_FULL;
	}
	// Creating an instance of the KeyEntry_t structure
	KeyEntry_t *keyEntry = (KeyEntry_t *)malloc(sizeof(KeyEntry_t));
	// Checking if Heap allocated space for the struct instance
	if(keyEntry == NULL)
	{
		log_error("Failed to allocate memory for Key Entry.");
		return KM_STATUS_NO_HEAP;
	}
	// Set struct memory to zero
	memset(keyEntry, 0, sizeof(KeyEntry_t));
	// Declare static arrays to hold Plaintext, Ciphertext, IV, HMAC and Buffer Array to flush to flash
	static uint8_t keyEntryArray[sizeof(KeyEntry_t)] = {0};
	static uint8_t plaintext[sizeof(KeyEntry_t)-53] = {0};
	static uint8_t ciphertext[sizeof(KeyEntry_t)-53] = {0};
	uint8_t iv[KEY_IV_SIZE] = {0};
	uint8_t hmac[KEY_HMAC_SIZE] = {0};
	uint8_t masterKey[16];
	int err = 0;
	// Set Key ID member in the structure
	keyEntry->key_id = keyID;
	// Set Key Size member in the structure
	keyEntry->key_size = keySize;
	// Set Key Origin member in the structure
	keyEntry->origin = keyOrigin;
	// Set Key Usage member in the structure
	keyEntry->usage = keyUsage;
	// Set Key Usage Count to zero in the structure
	keyEntry->usage_count = 0;
	// Generate a 16-byte Random Initialization Vector
	err = GenerateIV(iv);
	if(err)
	{
		log_error("IV Generation Failed.");
		return KM_STATUS_IV_FAIL;
	}
	// Encrypt the Key Entry Blob with a Master key from KDF
	err = CryptoEngine_Codec(ciphertext, sizeof(ciphertext), plaintext, sizeof(plaintext), iv, masterKey);
	if(err)
	{
		log_error("Codec Generation Failed.");
		return KM_STATUS_CODEC_FAIL;
	}
	// Generate the 32-byte HMAC digest for the encrypted Key Entry Blob
	err = GenerateHMAC(ciphertext, sizeof(ciphertext), masterKey, hmac);
	if(err)
	{
		log_error("HMAC Generation Failed.");
		return KM_STATUS_HMAC_FAIL;
	}
	// Set Key ID in the Buffer array
	keyEntryArray[KEY_ID_POS + 0] = (uint8_t)((keyEntry->key_id << 24) & 0xFF);
	keyEntryArray[KEY_ID_POS + 1] = (uint8_t)((keyEntry->key_id << 16) & 0xFF);
	keyEntryArray[KEY_ID_POS + 2] = (uint8_t)((keyEntry->key_id << 8) & 0xFF);
	keyEntryArray[KEY_ID_POS + 3] = (uint8_t)((keyEntry->key_id) & 0xFF);
	// Set Key Usage Count in the Buffer array
	keyEntryArray[KEY_USECOUNT_POS] = (uint8_t)((keyEntry->usage_count) & 0xFF);
	// Copy the encrypted Key Entry Blob into the Buffer array
	memcpy(&keyEntryArray[KEY_ORIGIN_POS], ciphertext, sizeof(ciphertext));
	// Copy the used Initialization vector into the Buffer array
	memcpy(&keyEntryArray[KEY_IV_POS], iv, sizeof(iv));
	// Copy the generated HMAC Digest into the Buffer array
	memcpy(&keyEntryArray[KEY_HMAC_POS], hmac, sizeof(hmac));
	// Call Flash Manager function to write to Flash
	if(FlashManager_WriteEntry(sectorNumber, keyEntryArray) != 0)
	{
		log_error("Write operation to Flash Failed.");
		return KM_STATUS_WRITE_KEY_FAIL;
	}
	log_info("Key Entry Added to Flash Memory.");
	return KM_STATUS_OK;
}

KeyManagerStatus_t KeyManager_GetKey(uint32_t keyID, uint8_t* key)
{
	// Find the Sector Number using the given Key ID
	uint16_t sectorNumber = KeyManager_FindKey(keyID);
	if(sectorNumber < 0)
	{
		log_error("Failed to match Key ID in Database.");
		return KM_STATUS_KEY_NOT_FOUND;

	}
	// Creating an instance of the KeyEntry_t structure
	KeyEntry_t *keyEntry = (KeyEntry_t *)malloc(sizeof(KeyEntry_t));
	// Checking if Heap allocated space for the struct instance
	if(keyEntry == NULL)
	{
		log_error("Failed to allocate memory for Key Entry.");
		return KM_STATUS_NO_HEAP;
	}
	// Set struct memory to zero
	memset(keyEntry, 0, sizeof(KeyEntry_t));
	// Declare static arrays to hold plaintext, ciphertext, iv, hmac and buffer array to flush to flash
	static uint8_t keyEntryArray[sizeof(KeyEntry_t)] = {0};
	static uint8_t plaintext[sizeof(KeyEntry_t)-53] = {0};
	static uint8_t ciphertext[sizeof(KeyEntry_t)-53] = {0};
	uint8_t iv[KEY_IV_SIZE] = {0};
	uint8_t stored_hmac[KEY_HMAC_SIZE] = {0};
	uint8_t computed_hmac[KEY_HMAC_SIZE] = {0};
	// 16-byte array to hold Derived Master Key
	uint8_t masterKey[16];
	int err = 0;
	// Call FlashManager function to read back the Key Entry and store it in keyEntryArray[]
	if(FlashManager_ReadEntry(sectorNumber, keyEntryArray) != 0)
	{
		log_error("Failed to Read entry from Flash.");
		return KM_STATUS_READ_KEY_FAIL;
	}
	// Copy the encrypted Key Entry Blob into the Ciphertext array
	memcpy(ciphertext, &keyEntryArray[KEY_ORIGIN_POS], sizeof(ciphertext));
	// Copy the used Initialization vector into the Buffer array
	memcpy(iv, &keyEntryArray[KEY_IV_POS], KEY_IV_SIZE);
	// Copy the generated HMAC Digest into the Buffer arrayS
	memcpy(stored_hmac, &keyEntryArray[KEY_HMAC_POS], KEY_HMAC_SIZE);
	// Generate the 32-byte HMAC digest for the encrypted Key Entry Blob
	err = GenerateHMAC(ciphertext, sizeof(ciphertext), masterKey, computed_hmac);
	if(err)
	{
		log_error("HMAC Generation Failed.");
		return KM_STATUS_HMAC_FAIL;
	}
	// Check if computed HMAC is same as stored value
	if(memcmp(stored_hmac, computed_hmac, KEY_HMAC_SIZE) != 0)
	{
		log_error("Computed HMAC value does not match with the stored value.");
		return KM_STATUS_HMAC_MISMATCH;
	}
	log_info("Computed HMAC value matches with the stored value.");
	// Decrypt the Key Entry Blob with a Master key from KDF
	err = CryptoEngine_Codec(plaintext, sizeof(plaintext), ciphertext, sizeof(ciphertext), iv, masterKey);
	if(err)
	{
		log_error("Codec Generation Failed.");
		return KM_STATUS_CODEC_FAIL;
	}
	// Retrieve the Key Size
	keyEntry->key_size = plaintext[KEY_SIZE_POS - 5];
	// Retrieve the Key Value
	memcpy(key, &plaintext[KEY_VAL_POS - 5], MAX_KEY_SIZE);
	// Update the Key Use Count in the Buffer array
	keyEntryArray[KEY_USECOUNT_POS] += 1;
	// Call FlashManager function to write the updated Buffer array to flash
	if(FlashManager_UpdateEntry(sectorNumber, keyEntryArray) != 0)
	{
		log_error("Failed to Update entry in Flash.");
		return KM_STATUS_UPDATE_KEY_FAIL;
	}
	// Return operation success flag.
	return KM_STATUS_OK;
}

KeyManagerStatus_t KeyManager_DeleteKey(uint32_t keyID)
{
	uint16_t sectorNumber = 0;
	uint8_t header[ENTRY_HEADER_SIZE] = {0};
	sectorNumber = KeyManager_FindKey(keyID);
	if(sectorNumber < 0)
	{
		log_error("Failed to match Key ID in Database.");
		return KM_STATUS_KEY_NOT_FOUND;
	}
	if(FlashManager_ReadIdentifier(sectorNumber, header) != 0)
	{
		log_error("Failed to read Identifier Flash.");
		return KM_STATUS_READ_KEY_FAIL;
	}
	if(header[VALIDITY_FLAG_POS] == VALID_ENTRY_FLAG)
	{
		log_info("Sector contains valid entry. Erasing now.");
		if(FlashManager_EraseEntry(sectorNumber) != 0)
		{
			log_error("Failed to Erase entry from Flash.");
			return KM_STATUS_DELETE_KEY_FAIL;
		}
	}
	return KM_STATUS_OK;
}
