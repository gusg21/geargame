#if !defined(GG_CODE_EDITOR_H)
#define GG_CODE_EDITOR_H

#include "../cimgui_impl_raylib.h"

typedef struct gg_asset gg_asset_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_asset_pair gg_asset_pair_t;
typedef struct gg_editor gg_editor_t;
typedef struct gg_state gg_state_t;
typedef struct gg_window gg_window_t;

typedef struct gg_code_editor {
    bool open;
    ImTextEditor text_editor;
    gg_asset_pair_t* text_editor_pair;
} gg_code_editor_t;

void CodeEditor_Create(gg_code_editor_t* code_editor);
void CodeEditor_Do(gg_code_editor_t* code_editor);
void CodeEditor_EditScriptAsset(gg_code_editor_t* code_editor, gg_asset_pair_t* asset_pair);
void CodeEditor_SaveScriptAsset(gg_code_editor_t* code_editor, gg_asset_pair_t* asset_pair);

#endif  // GG_CODE_EDITOR_H
