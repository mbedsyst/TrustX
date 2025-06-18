#include "DeviceInfo.h"
#include "Logger.h"
#include "stm32h5xx_hal.h"

InfoStatus_t DeviceInfo_GetDeviceID(uint32_t devID)
{
    uint8_t err = HAL_GetDEVID();
    if(err)
    {
    	log_error("Failed to retrieve Device ID.");
    	return DEVICE_INFO_DEV_ID_FAIL;
    }
	return DEVICE_INFO_OK;
}

InfoStatus_t DeviceInfo_GetUniqueID(uint8_t* uID)
{
    if (uID == NULL)
    {
    	log_error("Received NULL Input for UID storage.");
        return DEVICE_INFO_NULL_INPUT;
    }
    uint32_t uid0 = HAL_GetUIDw0(); // bits 0–31
    uint32_t uid1 = HAL_GetUIDw1(); // bits 32–63
    uint32_t uid2 = HAL_GetUIDw2(); // bits 64–95

    if((uid0 < 0) | (uid1 < 0) | (uid2 < 0))
    {
    	log_error("Failed to retrieve Unique ID.");
    	return DEVICE_INFO_UID_FAIL;
    }

    // Store in Little Endian order (byte-wise)
    uID[0]  = (uid0 >>  0) & 0xFF;
    uID[1]  = (uid0 >>  8) & 0xFF;
    uID[2]  = (uid0 >> 16) & 0xFF;
    uID[3]  = (uid0 >> 24) & 0xFF;

    uID[4]  = (uid1 >>  0) & 0xFF;
    uID[5]  = (uid1 >>  8) & 0xFF;
    uID[6]  = (uid1 >> 16) & 0xFF;
    uID[7]  = (uid1 >> 24) & 0xFF;

    uID[8]  = (uid2 >>  0) & 0xFF;
    uID[9]  = (uid2 >>  8) & 0xFF;
    uID[10] = (uid2 >> 16) & 0xFF;
    uID[11] = (uid2 >> 24) & 0xFF;

    return DEVICE_INFO_OK;
}
