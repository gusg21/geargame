#if !defined(GG_EDITOR_H)
#define GG_EDITOR_H

#include "cimgui_impl_raylib.h"

#include <stdbool.h>

typedef struct gg_state gg_state_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_window gg_window_t;
typedef struct gg_asset_pair gg_asset_pair_t;

typedef struct gg_editor {
    ImTextEditor text_editor;
    gg_asset_pair_t* text_editor_pair;

    bool is_state_viewer_open;
    bool is_assets_viewer_open;
    bool is_code_editor_open;
    bool is_asset_list_open;
    bool is_demo_window_open;
} gg_editor_t;

void Editor_Create(gg_editor_t* editor);
void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window);

#endif // GG_EDITOR_H
