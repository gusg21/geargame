#if !defined(GG_LUA_CONSOLE_T)
#define GG_LUA_CONSOLE_T

#define EDITOR_LUA_CONSOLE_LINES 30
#define EDITOR_LUA_MAX_LINE_LENGTH 256
#define EDITOR_LUA_INPUT_LINE_LENGTH 128

#include <stdbool.h>

#include "../color.h"
#include "../lua54/lua.h"
#include "ggwidgets.h"

typedef struct gg_assets gg_assets_t;
typedef struct gg_editor gg_editor_t;
typedef struct gg_scripting gg_scripting_t;
typedef struct gg_state gg_state_t;
typedef struct gg_window gg_window_t;

typedef struct gg_lua_console {
    bool open;
    gg_console_t console;
    char lua_input_line[EDITOR_LUA_INPUT_LINE_LENGTH];
} gg_lua_console_t;

void LuaConsole_Create(gg_lua_console_t* lua_console, gg_scripting_t* scripting);
void LuaConsole_ExecuteLuaLine(gg_lua_console_t* console, gg_scripting_t* script, const char* line);
void LuaConsole_Do(gg_lua_console_t* console, gg_scripting_t* scripting);
int LuaConsole_LuaPrint(lua_State* L);

#endif  // GG_LUA_CONSOLE_T
