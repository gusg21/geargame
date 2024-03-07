#if !defined(GG_OUTPUT_CONSOLE_T)
#define GG_OUTPUT_CONSOLE_T

#include <stdbool.h>

#include "../color.h"
#include "../lua54/lua.h"
#include "ggwidgets.h"

typedef struct gg_assets gg_assets_t;
typedef struct gg_editor gg_editor_t;
typedef struct gg_scripting gg_scripting_t;
typedef struct gg_state gg_state_t;
typedef struct gg_window gg_window_t;

typedef struct gg_output_console {
    gg_console_t console;
} gg_output_console_t;

void OutputConsole_Create(gg_output_console_t* console);
void OutputConsole_ExecuteCommand(gg_output_console_t* console, const char* command);
void OutputConsole_Do(gg_output_console_t* console);
void OutputConsole_Destroy(gg_output_console_t* console);

#endif  // GG_OUTPUT_CONSOLE_T
