#ifndef UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_
#define UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

#define RTC_TIMESTAMP_STRING_SIZE 32  // Enough for full date-time string

// Initializes the RTC (should be called once during system init)
void RTC_Wrapper_Init(void);

// Gets a formatted timestamp string: "YYYY-MM-DD HH:MM:SS"
void RTC_Wrapper_GetTimestamp(char *buffer, uint32_t buf_size);

#endif /* UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_ */
