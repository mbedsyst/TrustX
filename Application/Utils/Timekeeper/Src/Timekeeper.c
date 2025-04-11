#include "../../Timekeeper/Inc/Timekeeper.h"
#include <stdio.h>
#include <string.h>

extern RTC_HandleTypeDef hrtc;

void Timekeeper_Init(void)
{

}

void Timekeeper_Set_Time(uint8_t hour, uint8_t min, uint8_t sec)
{
    RTC_TimeTypeDef sTime = {0};
    sTime.Hours = hour;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

void Timekeeper_Set_Date(uint8_t date, uint8_t month, uint8_t year)
{
    RTC_DateTypeDef sDate = {0};
    sDate.Date = date;
    sDate.Month = month;
    sDate.Year = year;
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;  // Optional, HAL usually auto-assigns
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void Timekeeper_Get_Timestamp(char *buffer, int max_len)
{
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);  // Must be called after GetTime due to HAL quirk

    snprintf(buffer, max_len,
             "[20%02d-%02d-%02d %02d:%02d:%02d]",
             sDate.Year, sDate.Month, sDate.Date,
             sTime.Hours, sTime.Minutes, sTime.Seconds);
}
