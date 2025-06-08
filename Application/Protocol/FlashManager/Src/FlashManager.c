#include "../../FlashManager/Inc/FlashManager.h"
#include "constants.h"
#include "Logger.h"
#include <string.h>

/* ToDo	FlashDB_Init()
		1. For all 2048 sectors:
			- Read the first byte.
			- If valid_flag == 0xAA:
				- Read key_id and store in key_id -> sector map (RAM).
			- Else if valid_flag == 0x55:
				- Erase the sector (reclaim invalidated storage).

   ToDo FlashDB_AddKeyEntry()
		1. Scan all sectors until a free sector is found:
			- Read first byte of each sector (valid_flag).
			- If valid_flag != 0xAA (valid), mark as free.

		2. Populate KeyEntry_t structure:
			- Set valid_flag = 0xAA.
			- Fill in metadata fields (key_id, key_size, etc.).
			- Encrypt the key and store it in encrypted_key field.
			- If IV is used, generate and fill it in.
			- If tag is needed (e.g., for AES-GCM), compute and store it.

		3. Write the KeyEntry_t to the beginning of the sector.

		4. Update the RAM map: key_id -> sector number.

   ToDo FlashDB_UpdateKeyEntry()
		1. Look up the sector number for the given key_id from RAM map.

		2. Read the entire sector into a temporary KeyEntry_t.

		3. Modify only the fields that need updating (e.g., IV, purpose, tag).

		4. Erase the sector (sector erase required before rewrite in NOR flash).

		5. Write the updated KeyEntry_t back to the same sector.

   ToDo FlashDB_UseKeyEntry()
		1. Look up the sector number for the given key_id from RAM map.

		2. Read the KeyEntry_t structure from that sector.

		3. Decrypt the encrypted_key and return the key to caller.

		4. Increment the usage_count field.

		5. Erase the sector.

		6. Write back the modified KeyEntry_t to the same sector.

   ToDo FlashDB_DeleteKeyEntry()
		1. Look up the sector number for the given key_id from RAM map.

		2. Write 0x55 to the first byte (valid_flag) of that sector.

		3. Remove the key_id from the RAM map.

		4. On next initialization, this sector will be erased and reused.
*/
