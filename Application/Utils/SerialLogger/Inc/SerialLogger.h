#ifndef UTILS_SERIALLOGGER_INC_SERIALLOGGER_H_
#define UTILS_SERIALLOGGER_INC_SERIALLOGGER_H_

#include "stm32h5xx_hal.h"
#include <stdint.h>

typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} LogLevel;

void Log_Error(const char *fmt, ...);
void Log_Info(const char *fmt, ...);
void Log_Debug(const char *fmt, ...);

#endif /* UTILS_SERIALLOGGER_INC_SERIALLOGGER_H_ */
