#include "Timekeeper.h"
#include <stdio.h>
#include <string.h>

extern RTC_HandleTypeDef hrtc; // Provided by STM32CubeMX or defined globally

void RTC_Wrapper_Init(void) {
    // Normally configured via CubeMX. If needed, manually init here.
    // HAL_RTC_Init(&hrtc); // Uncomment if not done by CubeMX
}

void RTC_Wrapper_GetTimestamp(char *buffer, uint32_t buf_size) {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // Must be called after GetTime!

    snprintf(buffer, buf_size,
             "%04d-%02d-%02d %02d:%02d:%02d",
             2000 + sDate.Year,
             sDate.Month,
             sDate.Date,
             sTime.Hours,
             sTime.Minutes,
             sTime.Seconds);
}

