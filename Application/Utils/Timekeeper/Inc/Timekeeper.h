#ifndef UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_
#define UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_

#include "stm32h5xx_hal.h"

void Timekeeper_Init(void);
void Timekeeper_Set_Time(uint8_t hour, uint8_t min, uint8_t sec);
void Timekeeper_Set_Date(uint8_t date, uint8_t month, uint8_t year);
void Timekeeper_Get_Timestamp(char *buffer, int max_len);

#endif /* UTILS_TIMEKEEPER_INC_TIMEKEEPER_H_ */
