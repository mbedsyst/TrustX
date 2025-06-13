#include "KeyManager.h"

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
}

KeyManager_Op_t KeyManager_DeleteKey(uint8_t keyID)
{
	/* ToDo
	 * 1. Search for entry in Database using Key ID
	 * 2. Once a match is found, check the flash memory device for valid entry
	 * 3. If entry is valid, convert location into sector number
	 * 4. Call FlashManager function to erase that sector
	 *
	 * */
}
