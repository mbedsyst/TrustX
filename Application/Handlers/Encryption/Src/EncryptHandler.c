#include "../../Encryption/Inc/EncryptHandler.h"
#include "Logger.h"
#include "constants.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#include "stm32h5xx_hal.h"

OperationStatus_t EncryptHandler_Encrypt(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	/* ToDo
	 * 1. Parse out Encryption Key State from INPUT DATA
	 * 2. Parse out the Key Data Field from INPUT DATA.
	 * 3. If Key is not provided, generate a Key of required size & store in an array.
	 * 4. If Key is provided, store it into an array.
	 * 5. If Key ID is provided, search for the key from the flash, decrypt and store it in an array.
	 * 6. Generate an IV of required size.
	 * 7. Initialize the wolfSSL Encryption context.
	 * 8. Execute the Encryption operation.
	 * 9. Store the used Key into the Flash and increase the usage counter.
	 * 10. Append the IV to the end of the Key ID, as OUTPUT DATA
	 * 11. Append the Ciphertext to the end of the IV, as OUTPUT DATA
	 * 12. Assign the size of OUTPUT DATA.
	 * 13. Return the Response Packet
	 * */
	return OPERATION_UNKNOWN_ERROR;
}
