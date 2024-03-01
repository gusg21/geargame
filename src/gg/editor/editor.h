#ifdef GG_EDITOR

#if !defined(GG_EDITOR_H)
#define GG_EDITOR_H

#include "../cimgui_impl_raylib.h"

#include <stdbool.h>

typedef struct gg_state gg_state_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_window gg_window_t;
typedef struct gg_asset_pair gg_asset_pair_t;

#define EDITOR_LUA_CONSOLE_LINES 30
#define EDITOR_LUA_MAX_LINE_LENGTH 256
#define EDITOR_LUA_INPUT_LINE_LENGTH 128
#define EDITOR_NEW_ASSET_NAME_LENGTH 64

typedef struct gg_editor {
    bool is_code_editor_open;
    bool is_code_editor_visible;
    ImTextEditor text_editor;
    gg_asset_pair_t* text_editor_pair;

    bool is_scene_viewer_open;
    bool is_scene_viewer_visible;

    bool is_assets_viewer_open;
    bool is_assets_viewer_visible;
    char new_asset_name[EDITOR_NEW_ASSET_NAME_LENGTH];

    bool is_demo_window_open;
    bool is_demo_window_visible;

    bool is_lua_console_open;
    bool is_lua_console_visible;
    char** lua_console_lines;
    gg_color_t lua_console_line_colors[EDITOR_LUA_CONSOLE_LINES];
    char lua_input_line[EDITOR_LUA_INPUT_LINE_LENGTH];
} gg_editor_t;

void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting);
void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window);

#endif // GG_EDITOR_H

#endif // GG_EDITOR