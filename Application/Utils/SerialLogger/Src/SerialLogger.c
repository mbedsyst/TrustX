#include "SerialLogger.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LOG_BUFFER_SIZE 256

extern UART_HandleTypeDef huart2; // Ensure this is declared in your main or HAL config file

static void Log_Message(LogLevel level, const char *fmt, va_list args) {
    char buffer[LOG_BUFFER_SIZE];
    char final_msg[LOG_BUFFER_SIZE];

    // Format the main message
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // Choose prefix based on log level
    const char *prefix;
    switch (level) {
        case LOG_LEVEL_ERROR: prefix = "[ERROR]"; break;
        case LOG_LEVEL_INFO:  prefix = "[INFO] "; break;
        case LOG_LEVEL_DEBUG: prefix = "[DEBUG]"; break;
        default:              prefix = "";       break;
    }

    // Get timestamp in ms since boot
    uint32_t timestamp = HAL_GetTick();

    // Final message format: [1234 ms] [INFO] Your message
    snprintf(final_msg, sizeof(final_msg), "[%lu ms] %s %s\r\n", timestamp, prefix, buffer);

    // Send over UART
    HAL_UART_Transmit(&huart2, (uint8_t *)final_msg, strlen(final_msg), HAL_MAX_DELAY);
}

void Log_Error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Log_Message(LOG_LEVEL_ERROR, fmt, args);
    va_end(args);
}

void Log_Info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Log_Message(LOG_LEVEL_INFO, fmt, args);
    va_end(args);
}

void Log_Debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    Log_Message(LOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}

