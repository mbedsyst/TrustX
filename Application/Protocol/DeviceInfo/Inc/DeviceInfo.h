#ifndef PROTOCOL_DEVICEINFO_INC_DEVICEINFO_H_
#define PROTOCOL_DEVICEINFO_INC_DEVICEINFO_H_

#include <stdint.h>

#define UID_SIZE_BYTES 12  // 96 bits = 12 bytes

typedef enum
{
	DEVICE_INFO_OK = 0x00,
	DEVICE_INFO_NULL_INPUT,
	DEVICE_INFO_DEV_ID_FAIL,
	DEVICE_INFO_UID_FAIL
}InfoStatus_t;

/**
 * @brief Get the 32-bit Device ID
 * @return uint32_t Device ID
 */
InfoStatus_t DeviceInfo_GetDeviceID(uint32_t devID);

/**
 * @brief Get the 96-bit Unique ID of the device
 * @param uid Pointer to a 12-byte array to store the UID
 */
InfoStatus_t DeviceInfo_GetUniqueID(uint8_t* uID);

#endif /* PROTOCOL_DEVICEINFO_INC_DEVICEIDENTIFIER_H_ */
