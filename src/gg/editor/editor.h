#ifdef GG_EDITOR

#if !defined(GG_EDITOR_H)
#define GG_EDITOR_H

#include "../cimgui_impl_raylib.h"

#include <stdbool.h>

#include "asseteditor.h"
#include "codeeditor.h"
#include "luaconsole.h"
#include "outputconsole.h"

#define ICON_GG_ADD ICON_ADD
#define ICON_GG_CLOSE ICON_CLOSE
#define ICON_GG_SAVE ICON_SAVE
#define ICON_GG_WARNING ICON_WARNING
#define ICON_GG_EDIT ICON_EDIT
#define ICON_GG_UNKNOWN ICON_QUESTION_MARK
#define ICON_GG_SPECIAL ICON_STAR
#define ICON_GG_PAUSE ICON_PAUSE
#define ICON_GG_PLAY ICON_PLAY_ARROW

#define ICON_GG_ASSET ICON_WIDGETS
#define ICON_GG_LUA ICON_TERMINAL
#define ICON_GG_OUTPUT ICON_TERMINAL
#define ICON_GG_IMGUI ICON_SETTINGS
#define ICON_GG_GAME ICON_GAMEPAD

#define ICON_GG_ACTOR_SPEC ICON_TYPE_SPECIMEN
#define ICON_GG_SCRIPT ICON_CODE
#define ICON_GG_SCENE ICON_FORMAT_LIST_NUMBERED
#define ICON_GG_TEXTURE ICON_IMAGE
#define ICON_GG_TILED_MAP ICON_GRID_VIEW

typedef struct gg_state gg_state_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_window gg_window_t;
typedef struct gg_asset_pair gg_asset_pair_t;

typedef struct gg_editor {
    bool is_code_editor_visible;
    gg_code_editor_t code_editor;

    bool is_scene_viewer_visible;

    bool is_assets_viewer_visible;
    gg_asset_editor_t asset_editor;

    bool is_demo_window_visible;

    bool is_lua_console_visible;
    gg_lua_console_t lua_console;

    bool is_output_console_visible;
    gg_output_console_t output_console;
} gg_editor_t;

// Create the Geargame Editor. Needs the scripting context to set up the Lua Console print hooks
void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting);
// Update + draw the editor using IMGUI.
void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window);
// Clean up the editor
void Editor_Destroy(gg_editor_t* editor);

#endif  // GG_EDITOR_H

#endif  // GG_EDITOR