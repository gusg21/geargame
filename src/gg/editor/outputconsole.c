#include "outputconsole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

gg_output_console_t* g_output_console;

static void OutputConsole_TraceLogCallback(int logLevel, const char* text, va_list args) {
    char line[1024] = {0};
    va_list args2;
    va_copy(args2, args);
    vsprintf_s(line, 1024, TextFormat("%d: %%s", logLevel), args2);
    printf("%s\n", line);

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

void OutputConsole_Create(gg_output_console_t* console) {
    console->open = true;
    g_output_console = console;
    GGWidgets_Console_Create(&console->console, 50);
    SetTraceLogCallback(OutputConsole_TraceLogCallback);
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