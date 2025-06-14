#include "KeyManager.h"

#define KEY_ID_POS			0
#define KEY_USECOUNT_POS	4
#define KEY_ORIGIN_POS		5
#define KEY_USAGE_POS		6
#define KEY_SIZE_POS		7
#define KEY_VAL_POS			8
#define KEY_IV_POS			40
#define KEY_HMAC_POS		56

#define KEY_ID_SIZE			4
#define KEY_USECOUNT_SIZE	1
#define KEY_ORIGIN_SIZE		1
#define KEY_USAGE_SIZE		1
#define KEY_SIZE_SIZE		1
#define KEY_VAL_SIZE		32
#define KEY_IV_SIZE			16
#define KEY_HMAC_SIZE		32

KeyManager_Op_t KeyManager_AddKey(uint8_t* keyID, uint8_t* key, uint8_t keySize, uint8_t keyOrigin, uint8_t keyUsage)
{
	/* ToDo
	 * 1. Create dynamic instance of Key Structure
	 * 2. Assign Key ID, Key Length and Key Size
	 * 3. Assign Key Origin and Key Usage
	 * 4. Set Key Usage Count to 0
	 * 5. Fill up an array with Key Metadata excluding Usage Count
	 * 6. Calculate HMAC-SHA256 of this Key Metadata array
	 * 7. Encrypt this Key Metadata array
	 * 8. Transform struct to array
	 * 9. Populate the Flash Memory to-write buffer
	 * 10. Call the FlashManager function
	 * */
	// Creating an instance of the KeyEntry_t structure
	KeyEntry_t *keyEntry = (KeyEntry_t *)malloc(sizeof(KeyEntry_t));
	// Checking if Heap allocated space for the struct instance
	if(keyEntry == NULL)
	{
		log_error("Failed to allocate memory for Key Entry.");
		return KM_STATUS_NO_HEAP;
	}
	// Set struct memory to zero
	memset(&keyEntry, 0, sizeof(KeyEntry_t));
	// Declare static arrays to hold plaintext, ciphertext, iv, hmac and buffer array to flush to flash
	static uint8_t keyEntryArray[sizeof(KeyEntry_t)] = {0};
	static uint8_t plaintext[sizeof(KeyEntry_t)-53] = {0};
	static uint8_t ciphertext[sizeof(KeyEntry_t)-53] = {0};
	uint8_t iv[KEY_IV_SIZE] = {0};
	uint8_t hmac[KEY_HMAC_SIZE] = {0};
	uint8_t masterKey[16];
	// Set Key ID member in the structure
	keyEntry->key_id = (keyID[0] << 24) | (keyID[1] << 16) | (keyID[2] << 8) | (keyID[3]);
	// Set Key Size member in the structure
	keyEntry->key_size = keySize;
	// Set Key Origin member in the structure
	keyEntry->origin = keyOrigin;
	// Set Key Usage member in the structure
	keyEntry->usage = keyUsage;
	// Set Key Usage Count to zero in the structure
	keyEntry->usage_count = 0;
	// Set Key ID in the Buffer array
	keyEntryArray[KEY_ID_POS + 0] = (uint8_t)((keyEntry->key_id<<24) & 0xFF);
	keyEntryArray[KEY_ID_POS + 1] = (uint8_t)((keyEntry->key_id<<16) & 0xFF);
	keyEntryArray[KEY_ID_POS + 2] = (uint8_t)((keyEntry->key_id<<8) & 0xFF);
	keyEntryArray[KEY_ID_POS + 3] = (uint8_t)((keyEntry->key_id) & 0xFF);
	// Set Key Usage Count in the Buffer array
	keyEntryArray[KEY_USECOUNT_POS] = (uint8_t)((keyEntry->usage_count)&0xFF);
	// Generate a 16-byte Random Initialization Vector
	GenerateIV(iv);
	// Encrypt the Key Entry Blob with a Master key from KDF
	CryptoEngine_Codec(ciphertext, sizeof(ciphertext), plaintext, sizeof(plaintext), iv, masterKey);
	// Generate the 32-byte HMAC digest for the encrypted Key Entry Blob
	GenerateHMAC(ciphertext, sizeof(ciphertext), masterKey, hmac);
	// Copy the encrypted Key Entry Blob into the Buffer array
	memcpy(&keyEntryArray[KEY_ORIGIN_POS], ciphertext, sizeof(ciphertext));
	// Copy the used Initialization vector into the Buffer array
	memcpy(&keyEntryArray[KEY_IV_POS], iv, sizeof(iv));
	// Copy the generated HMAC Digest into the Buffer arrayS
	memcpy(&keyEntryArray[KEY_HMAC_POS], hmac, sizeof(hmac));

	// ToDo Call Flash Manager function to write to Flash

	return KM_STATUS_OK;
}

KeyManager_Op_t KeyManager_GetKey(uint8_t* keyID, uint8_t* key)
{
	/* ToDo
	 * 1. Create dynamic instance of Key Structure
	 * 2. Call FlashManager function to read entry using Key ID
	 * 3. Parse out ciphertext and MAC from buffer
	 * 4. Decrypt ciphertext and compute MAC
	 * 5. Verify the computed MAC with stored value
	 * 6. Transform array & Populate the key structure with entry values
	 * 7. Copy key into the function argument
	 * 8. Update Usage Count member in the structure
	 * 9. Copy the read-back ciphertext content and MAC to the structure
	 * 10. Transform structure back into array
	 * 11. Call the FLash Manager function
	 * */
	// Creating an instance of the KeyEntry_t structure
	KeyEntry_t *keyEntry = (KeyEntry_t *)malloc(sizeof(KeyEntry_t));
	// Checking if Heap allocated space for the struct instance
	if(keyEntry == NULL)
	{
		log_error("Failed to allocate memory for Key Entry.");
		return KM_STATUS_NO_HEAP;
	}
	// Set struct memory to zero
	memset(&keyEntry, 0, sizeof(KeyEntry_t));

	// Declare static arrays to hold plaintext, ciphertext, iv, hmac and buffer array to flush to flash
	static uint8_t keyEntryArray[sizeof(KeyEntry_t)] = {0};
	static uint8_t plaintext[sizeof(KeyEntry_t)-53] = {0};
	static uint8_t ciphertext[sizeof(KeyEntry_t)-53] = {0};
	uint8_t iv[KEY_IV_SIZE] = {0};
	uint8_t stored_hmac[KEY_HMAC_SIZE] = {0};
	uint8_t computed_hmac[KEY_HMAC_SIZE] = {0};
	uint8_t masterKey[16];

	// ToDo Call FlashManager function to read back the Key Entry and store it in keyEntryArray[]

	// Copy the encrypted Key Entry Blob into the Ciphertext array
	memcpy(ciphertext, &keyEntryArray[KEY_ORIGIN_POS], sizeof(ciphertext));
	// Copy the used Initialization vector into the Buffer array
	memcpy(iv, &keyEntryArray[KEY_IV_POS], sizeof(iv));
	// Copy the generated HMAC Digest into the Buffer arrayS
	memcpy(stored_hmac, &keyEntryArray[KEY_HMAC_POS], sizeof(hmac));
	// Generate the 32-byte HMAC digest for the encrypted Key Entry Blob
	GenerateHMAC(ciphertext, sizeof(ciphertext), masterKey, hmac);
	// Check if computed HMAC is same as stored value
	if(memcmp(stored_hmac, computed_hmac, KEY_HMAC_SIZE) != 0)
	{
		log_error("Computed HMAC value does not match with the stored value.");
		return KM_STATUS_ERROR;
	}
	log_info("Computed HMAC value matches with the stored value.");
	// Decrypt the Key Entry Blob with a Master key from KDF
	CryptoEngine_Codec(plaintext, sizeof(plaintext), ciphertext, sizeof(ciphertext), iv, masterKey);
	// Set Key ID member in the structure
	keyEntry->key_id = (keyEntryArray[KEY_ID_POS + 0] << 24)
					 | (keyEntryArray[KEY_ID_POS + 1] << 16)
					 | (keyEntryArray[KEY_ID_POS + 2] << 8)
					 | (keyEntryArray[KEY_ID_POS + 3]);
	// Retrieve the Key Usage Count
	keyEntry->usage_count = keyEntryArray[KEY_USECOUNT_POS];
	// Retrieve the Key Origin
	keyEntry->origin = plaintext[KEY_ORIGIN_POS - 5];
	// Retrieve the Set Key Usage
	keyEntry->usage = plaintext[KEY_USAGE_POS - 5];
	// Retrieve the Key Size
	keyEntry->key_size = plaintext[KEY_SIZE_POS - 5];
	// Retrieve the Key Value
	memcpy(&keyEntry->key_val, plaintext[KEY_VAL_POS - 5], MAX_KEY_SIZE);
	// Retrieve the Key Blob IV
	memcpy(&keyEntry->iv, keyEntryArray[KEY_IV_POS], KEY_IV_SIZE);
	// Retrieve the Key Blob HMAC
	memcpy(&keyEntry->hmac, keyEntryArray[KEY_HMAC_POS], KEY_HMAC_SIZE);

	// Update the Key Use Count in the Buffer array
	keyEntryArray[KEY_USECOUNT_POS] += 1;
	// ToDo Call FlashManager function to write the updated Buffer array to flash

	return KM_OP_NONE;
}

KeyManager_Op_t KeyManager_DeleteKey(uint8_t keyID)
{
	/* ToDo
	 * 1. Search for entry in Database using Key ID
	 * 2. Once a match is found, check the flash memory device for valid entry
	 * 3. If entry is valid, convert location into sector number
	 * 4. Call FlashManager function to erase that sector
	 * */
	return KM_OP_NONE;
}
