#ifndef PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_
#define PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_

#include <stdint.h>
#include <stdbool.h>

#define SALT_LENGTH        16  // You can change this if needed

bool SaltManager_Init(void);
const uint8_t* SaltManager_Get(void);

#endif /* PROTOCOL_SALTMANAGER_INC_SALTMANAGER_H_ */
