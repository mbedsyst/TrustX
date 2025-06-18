#include "../../Logger/Inc/Logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// ANSI color codes
#define COLOR_DEBUG "\033[0;36m"  // Cyan
#define COLOR_INFO  "\033[0;32m"  // Green
#define COLOR_WARN  "\033[0;33m"  // Yellow
#define COLOR_ERROR "\033[0;31m"  // Red
#define COLOR_RESET "\033[0m"

#define LOG_BUFFER_SIZE 256

static LogLevel current_level = LOG_LEVEL_INFO;

void log_init(LogLevel level)
{
    current_level = level;
}

void log_clearline(void)
{
    // Move cursor up and clear the entire line
    printf("\033[A\033[2K\r");
}

static void log_generic(LogLevel level, const char *prefix, const char *color, const char *fmt, va_list args)
{
    if (level < current_level)
        return;

    char buffer[LOG_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    // Print directly using printf(), assumes it's redirected to VCOM via BSP
    printf("%s[%s] %s%s\r\n", color, prefix, buffer, COLOR_RESET);
}

void log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_generic(LOG_LEVEL_DEBUG, "DEBUG", COLOR_DEBUG, fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_generic(LOG_LEVEL_INFO, "INFO", COLOR_INFO, fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_generic(LOG_LEVEL_WARN, "WARN", COLOR_WARN, fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log_generic(LOG_LEVEL_ERROR, "ERROR", COLOR_ERROR, fmt, args);
    va_end(args);
}
