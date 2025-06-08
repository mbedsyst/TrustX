#include "../../Decryption/Inc/DecryptHandler.h"
#include "Logger.h"
#include "constants.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#include "stm32h5xx_hal.h"

OperationStatus_t DecryptHandler_Decrypt(const ParsedPacket_t* request, ResponsePacket_t* response)
{
	/* ToDo
	 * 1. Parse out Decryption Key State from INPUT DATA
	 * 2. Parse out the Key Data field from INPUT DATA.
	 * 3. Parse out IV State from INPUT DATA
	 * 4. Parse out the IV Data field from INPUT DATA
	 * 5. If Key is provided, store it into an array.
	 * 6. If Key ID is provided, search for the key from the flash, decrypt and store it in an array.
	 * 7. Store the provided IV into an array
	 * 8. Initialize the wolfSSL Decryption context.
	 * 9. Execute the Decryption operation.
	 * 10. Store the used Key into the Flash and increase the usage counter.
	 * 11. Append the Plaintext to the end of the Key ID, as OUTPUT DATA
	 * 12. Assign the size of OUTPUT DATA.
	 * 13. Return the Response Packet
	 * */
	return OPERATION_UNKNOWN_ERROR;
}
