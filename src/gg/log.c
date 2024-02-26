#include "log.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void Log_Info(const char* info) { TraceLog(LOG_INFO, info); }

void Log_Warn(const char* warning) { TraceLog(LOG_WARNING, warning); }

void Log_Err(const char* error) { TraceLog(LOG_ERROR, error); }

char* Log_TextFormat(const char* format, ...) {
    char* buffer = (char*)malloc(sizeof(char) * 256);

    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, 256, format, args);
    va_end(args);

    return buffer;
}
