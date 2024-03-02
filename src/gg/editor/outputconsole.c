#include "outputconsole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../utils.h"

gg_output_console_t* g_output_console = NULL;

static void OutputConsole_TraceLogCallback(int logLevel, const char* text, va_list args) {
    char line[WIDGETS_CONSOLE_MAX_LINE_LEN] = {0};

    // Construct the time and date info
    char timeStr[64] = {0};
    time_t now = time(NULL);
    struct tm tm_info;
    localtime_s(&tm_info, &now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm_info);
    printf_s("[%s] ", timeStr);

    switch (logLevel) {
        case LOG_INFO:
            printf_s("[INFO] : ");
            break;
        case LOG_ERROR:
            printf_s("[ERROR]: ");
            break;
        case LOG_WARNING:
            printf_s("[WARN] : ");
            break;
        case LOG_DEBUG:
            printf_s("[DEBUG]: ");
            break;
        default:
            break;
    }

    // Print the actual info
    vprintf_s(text, args);
    vsprintf_s(line, WIDGETS_CONSOLE_MAX_LINE_LEN, text, args);

    // Only need new line for stdout
    printf("\n");

    gg_color_t color;
    switch (logLevel) {
        case LOG_INFO:
            color = COL(255, 255, 255);
            break;
        case LOG_WARNING:
            color = COL(255, 200, 0);
            break;
        case LOG_ERROR:
            color = COL(255, 0, 0);
            break;
        case LOG_FATAL:
            color = COL(230, 0, 50);
            break;
    }

    GGWidgets_Console_AppendLineColored(&g_output_console->console, line, color);
}

void OutputConsole_OverrideTraceLog(gg_output_console_t* console) {
    SetTraceLogCallback(OutputConsole_TraceLogCallback);
}

void OutputConsole_Create(gg_output_console_t* console) {
    console->open = true;
    g_output_console = console;
    GGWidgets_Console_Create(&console->console, 50);
    OutputConsole_OverrideTraceLog(console);
}

void OutputConsole_ExecuteCommand(gg_output_console_t* console, const char* line) {
    GGWidgets_Console_AppendLine(&console->console, TextFormat("> %s", line));
}

void OutputConsole_Do(gg_output_console_t* console) {
    if (igBegin("Output Console", &console->open, 0)) {
        GGWidgets_Console_Do(&console->console, false);
    }
    igEnd();
}