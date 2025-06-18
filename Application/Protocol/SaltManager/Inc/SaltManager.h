#ifndef PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_
#define PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_

#include <stdint.h>
#include <stdbool.h>

#define SALT_LENGTH        16  // You can change this if needed

typedef enum
{
	SALT_MANAGER_OK = 0,
	SALT_MANAGER_INIT_FAIL,
	SALT_MANAGER_GENERATOR_FAIL,
	SALT_MANAGER_SALT_NULL,
	SALT_MANAGER_FLASH_ERASE_FAIL,
	SALT_MANAGER_FLASH_WRITE_FAIL

}SaltManagerStatus_t;

SaltManagerStatus_t SaltManager_Init(void);
const uint8_t* SaltManager_Get(void);

#endif /* PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_ */
