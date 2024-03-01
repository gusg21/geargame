#ifdef GG_EDITOR

#include <string.h>

#include "../actor.h"
#include "../assets.h"
#include "../state.h"
#include "../utils.h"
#include "editor.h"
#include "ggwidgets.h"

static gg_editor_t* g_editor;

static void Editor_S_AppendLuaConsoleLineColored(gg_editor_t* editor, const char* text, gg_color_t color) {
    char* new_line = calloc(EDITOR_LUA_MAX_LINE_LENGTH, sizeof(char));
    sprintf_s(new_line, EDITOR_LUA_MAX_LINE_LENGTH, "%s", text);

    free(editor->lua_console_lines[EDITOR_LUA_CONSOLE_LINES - 1]);  // Clear the last one

    for (size_t i = EDITOR_LUA_CONSOLE_LINES - 1; i > 0; i--) {
        editor->lua_console_lines[i] = editor->lua_console_lines[i - 1];
        editor->lua_console_line_colors[i] = editor->lua_console_line_colors[i - 1];
    }

    editor->lua_console_lines[0] = new_line;
    editor->lua_console_line_colors[0] = color;
}

static void Editor_S_AppendLuaConsoleLine(gg_editor_t* editor, const char* text) {
    Editor_S_AppendLuaConsoleLineColored(editor, text, COL(255, 255, 255));
}

static void Editor_S_TraceLogCallback(int logLevel, const char* text, va_list args) {
    char line[1024] = {0};
    sprintf_s(line, 1024, "%d: %s", logLevel, text);
    printf("%s\n", line);
}

static int Editor_S_LuaPrint(lua_State* L) {
    const char* text = lua_tostring(L, 1);

    printf("%s\t", text);
    Editor_S_AppendLuaConsoleLine(g_editor, text);
    printf("\n");

    return 0;
}

void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting) {
    g_editor = editor;

    AssetEditor_Create(&editor->asset_editor);
    CodeEditor_Create(&editor->code_editor);

    editor->is_scene_viewer_visible = true;
    editor->is_assets_viewer_visible = true;
    editor->lua_console_lines = calloc(EDITOR_LUA_CONSOLE_LINES, sizeof(char*));

    SetTraceLogCallback(Editor_S_TraceLogCallback);
    lua_register(scripting->state, "print", Editor_S_LuaPrint);
}

static void Editor_S_EditScriptAsset(gg_editor_t* editor, gg_asset_pair_t* pair) {
    
}

static void Editor_S_ToggleButton(const char* label, bool* value) {
    bool highlight = *value;
    if (highlight) igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){92 / 255.f, 176 / 255.f, 214 / 255.f, 255 / 255.f});
    if (igButton2(label)) {
        *value = !*value;
    }
    if (highlight) igPopStyleColor(1);
}

static void Editor_S_DoSceneViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImGuiIO* io = igGetIO();
    igBegin("Scene Viewer", &editor->is_scene_viewer_open, 0);
    {
        igSeparator();

        GGWidgets_Header(editor, (ImVec4){235 / 255.f, 185 / 255.f, 72 / 255.f, 255 / 255.f}, "Scene Actors");

        const uint32_t max_script_names = 64;
        char** script_names = calloc(max_script_names, sizeof(char*));
        Assets_FindAssetsByType(assets, script_names, ASSET_SCRIPT);

        // For each actor
        gg_scene_t* scene = state->current_scene;
        for (uint32_t actor_id = 0; actor_id < SCENE_MAX_ACTORS; actor_id++) {
            gg_actor_t* actor = &scene->actors[actor_id];
            if (actor->alive) {
                igText("[%d] %s", actor_id, actor->name);
                igSameLine2();
                igSetCursorPosX(140);
                igPushID_Int(actor_id);
                if (igCollapsingHeader_Str("Details")) {
                    igIndent2();
                    GGWidgets_Actor(editor, actor, actor_id, script_names, state, assets, window);
                    igUnindent2();
                }
                igPopID();
                igSpacing();
            }
        }

        free(script_names);
    }
    igEnd();
}

static void Editor_S_DoToolbar(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    bool open = true;

    igBegin("##Toolbar", &open,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        igSetWindowPos_Vec2((ImVec2){0, 0}, ImGuiCond_Always);
        igSetWindowSize_Vec2((ImVec2){(float)GetScreenWidth(), 36}, ImGuiCond_Always);

        if (igButton2("Quit")) {
            State_Quit(state);
        }
        igSameLine2();

        igText("gg engine");
        igSameLine2();

        igText("-");
        igSameLine2();

        Editor_S_ToggleButton("Scene", &editor->is_scene_viewer_visible);
        igSameLine2();

        Editor_S_ToggleButton("Code", &editor->is_code_editor_visible);
        igSameLine2();

        Editor_S_ToggleButton("Assets", &editor->is_assets_viewer_visible);
        igSameLine2();

        Editor_S_ToggleButton("Lua", &editor->is_lua_console_visible);
        igSameLine2();

        Editor_S_ToggleButton("ImGui Demo", &editor->is_demo_window_visible);
        igSameLine2();

        igText("FPS %.2f (ft %.2f)", 1.f / Window_GetDeltaTime(window), Window_GetDeltaTime(window));
    }
    igEnd();
}

static void Editor_S_ExecuteLuaLine(gg_editor_t* editor, gg_scripting_t* script) {
    Editor_S_AppendLuaConsoleLine(editor, TextFormat("> %s", editor->lua_input_line));

    int32_t error = Scripting_RunCode(script, editor->lua_input_line);
    if (error > 0) {
        Editor_S_AppendLuaConsoleLineColored(editor, TextFormat("%s", Scripting_GetError(script)), COL(255, 50, 50));
        Editor_S_AppendLuaConsoleLineColored(editor, TextFormat("err %d", error), COL(120, 0, 0));
    }

    memset(editor->lua_input_line, 0, EDITOR_LUA_INPUT_LINE_LENGTH);
}

static void Editor_S_DoLuaConsole(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImVec2 size;
    igCalcTextSize(&size, "TALL LETTERS", NULL, false, 0);
    float height = igGetStyle()->ItemSpacing.y + size.y;
    igSetNextWindowSizeConstraints2((ImVec2){0.f, 0.f}, (ImVec2){FLT_MAX, (EDITOR_LUA_CONSOLE_LINES + 4) * height});
    if (igBegin("Lua Console", &editor->is_lua_console_open, 0)) {
        igBeginChild_ID(1791, (ImVec2){0, -30}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        for (int32_t i = EDITOR_LUA_CONSOLE_LINES - 1; i >= 0; i--) {
            if (editor->lua_console_lines[i]) {
                igTextColored(
                    (ImVec4){editor->lua_console_line_colors[i].r / 255.f, editor->lua_console_line_colors[i].g / 255.f,
                             editor->lua_console_line_colors[i].b / 255.f,
                             editor->lua_console_line_colors[i].a / 255.f},
                    "%s", editor->lua_console_lines[i]);
            } else {
                igText("");
            }
        }
        igEndChild();

        if (igInputText("<", editor->lua_input_line, 127, ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL)) {
            igSetKeyboardFocusHere(-1);
            Editor_S_ExecuteLuaLine(editor, &state->current_scene->scripting);
        }
        igSameLine2();
        if (igButton2("Enter")) {
            Editor_S_ExecuteLuaLine(editor, &state->current_scene->scripting);
        }
    }
    igEnd();
}

void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_S_DoToolbar(editor, state, assets, window);

    if (editor->is_scene_viewer_visible) Editor_S_DoSceneViewer(editor, state, assets, window);
    if (editor->is_assets_viewer_visible) AssetEditor_Do(&editor->asset_editor, editor, state, assets, window);
    if (editor->is_code_editor_visible) CodeEditor_Do(&editor->code_editor);
    if (editor->is_lua_console_visible) Editor_S_DoLuaConsole(editor, state, assets, window);
    if (editor->is_demo_window_visible) igShowDemoWindow(&editor->is_demo_window_open);
}

#endif  // GG_EDITOR