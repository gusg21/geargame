#ifdef GG_EDITOR

#if !defined(GG_EDITOR_H)
#define GG_EDITOR_H

#include "../cimgui_impl_raylib.h"

#include <stdbool.h>

#include "asseteditor.h"
#include "codeeditor.h"
#include "luaconsole.h"
#include "outputconsole.h"

typedef struct gg_state gg_state_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_window gg_window_t;
typedef struct gg_asset_pair gg_asset_pair_t;

typedef struct gg_editor {
    bool is_code_editor_visible;
    gg_code_editor_t code_editor;

    bool is_scene_viewer_open;
    bool is_scene_viewer_visible;

    bool is_assets_viewer_visible;
    gg_asset_editor_t asset_editor;

    bool is_demo_window_open;
    bool is_demo_window_visible;

    bool is_lua_console_visible;
    gg_lua_console_t lua_console;

    bool is_output_console_visible;
    gg_output_console_t output_console;
    
} gg_editor_t;

void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting);
void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window);

#endif // GG_EDITOR_H

#endif // GG_EDITOR