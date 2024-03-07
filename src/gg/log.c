#include "log.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "utils.h"

void Log_Info(const char* info) { TraceLog(LOG_INFO, info); }

void Log_Warn(const char* warning) { TraceLog(LOG_WARNING, warning); }

void Log_Err(const char* error) { TraceLog(LOG_ERROR, error); }

char* Log_TextFormat(const char* format, ...) {
#ifndef MAX_TEXTFORMAT_BUFFERS
#define MAX_TEXTFORMAT_BUFFERS 4  // Maximum number of static buffers for text formatting
#endif
#ifndef MAX_TEXT_BUFFER_LENGTH
#define MAX_TEXT_BUFFER_LENGTH 1024  // Maximum size of static text buffer
#endif

    // We create an array of buffers so strings don't expire until MAX_TEXTFORMAT_BUFFERS invocations
    static char buffers[MAX_TEXTFORMAT_BUFFERS][MAX_TEXT_BUFFER_LENGTH] = {0};
    static int index = 0;

    char* currentBuffer = buffers[index];
    memset(currentBuffer, 0, MAX_TEXT_BUFFER_LENGTH);  // Clear buffer before using

    va_list args;
    va_start(args, format);
    int requiredByteCount = vsnprintf(currentBuffer, MAX_TEXT_BUFFER_LENGTH, format, args);
    va_end(args);

    // If requiredByteCount is larger than the MAX_TEXT_BUFFER_LENGTH, then overflow occured
    if (requiredByteCount >= MAX_TEXT_BUFFER_LENGTH) {
        // Inserting "..." at the end of the string to mark as truncated
        char* truncBuffer = buffers[index] + MAX_TEXT_BUFFER_LENGTH - 4;  // Adding 4 bytes = "...\0"
        sprintf_s(truncBuffer, 4, "...", NULL);
    }

    index += 1;  // Move to next buffer for next function call
    if (index >= MAX_TEXTFORMAT_BUFFERS) index = 0;

    return currentBuffer;
}
