#ifdef GG_EDITOR

#include "luaconsole.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lua54/lua.h"
#include "../scripting.h"
#include "../state.h"
#include "../utils.h"
#include "editor.h"

gg_lua_console_t* g_lua_console;

void LuaConsole_Create(gg_lua_console_t* lua_console, gg_scripting_t* scripting) {
    lua_console->open = true;
    g_lua_console = lua_console;
    GGWidgets_Console_Create(&lua_console->console, EDITOR_LUA_CONSOLE_LINES);
    lua_register(scripting->state, "print", LuaConsole_LuaPrint);
}

void LuaConsole_ExecuteLuaLine(gg_lua_console_t* console, gg_scripting_t* script, const char* line) {
    GGWidgets_Console_AppendLine(&console->console, TextFormat("> %s", line));

    int32_t error = Scripting_RunCode(script, line);
    if (error > 0) {
        GGWidgets_Console_AppendLineColored(&console->console, TextFormat("%s", Scripting_GetError(script)), COL(255, 50, 50));
        GGWidgets_Console_AppendLineColored(&console->console, TextFormat("err %d", error), COL(120, 0, 0));
    }
}

void LuaConsole_Do(gg_lua_console_t* console, gg_scripting_t* scripting) {
    ImVec2 size;
    igCalcTextSize(&size, "TALL LETTERS", NULL, false, 0);
    float height = igGetStyle()->ItemSpacing.y + size.y;
    igSetNextWindowSizeConstraints2((ImVec2){0.f, 0.f}, (ImVec2){FLT_MAX, (EDITOR_LUA_CONSOLE_LINES + 4) * height});
    if (igBegin(ICON_GG_LUA " Lua Console", &console->open, 0)) {
        GGWidgets_Console_Do(&console->console, true);

        if (igInputText("<", console->lua_input_line, 127, ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL)) {
            igSetKeyboardFocusHere(-1);
            LuaConsole_ExecuteLuaLine(console, scripting, console->lua_input_line);
            memset(console->lua_input_line, 0, EDITOR_LUA_INPUT_LINE_LENGTH);
        }
        igSameLine2();
        if (igButton2("Enter")) {
            LuaConsole_ExecuteLuaLine(console, scripting, console->lua_input_line);
            memset(console->lua_input_line, 0, EDITOR_LUA_INPUT_LINE_LENGTH);
        }
    }
    igEnd();
}

int LuaConsole_LuaPrint(lua_State* L) {
    const char* text = lua_tostring(L, 1);

    TraceLog(LOG_INFO, "%s", text);
    GGWidgets_Console_AppendLineColored(&g_lua_console->console, text, COL(128, 180, 255));

    return 0;
}

void LuaConsole_Destroy(gg_lua_console_t* console) {
    GGWidgets_Console_Destroy(&console->console);
}

#endif