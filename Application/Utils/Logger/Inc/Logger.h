#ifndef UTILS_LOGGER_INC_LOGGER_H_
#define UTILS_LOGGER_INC_LOGGER_H_

#include "stm32h5xx_hal.h"

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

void log_init(LogLevel level);
void log_debug(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_clearline(void);


#endif /* UTILS_LOGGER_INC_LOGGER_H_ */
