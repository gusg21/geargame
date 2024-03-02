#if !defined(GG_ASSET_EDITOR_H)
#define GG_ASSET_EDITOR_H

#define EDITOR_NEW_ASSET_NAME_LENGTH 64

#include <stdbool.h>

typedef struct gg_assets gg_assets_t;
typedef struct gg_asset_pair gg_asset_pair_t;
typedef struct gg_code_editor gg_code_editor_t;
typedef struct gg_editor gg_editor_t;
typedef struct gg_state gg_state_t;
typedef struct gg_window gg_window_t;

typedef struct gg_asset_editor {
    bool open;
    char new_asset_name[EDITOR_NEW_ASSET_NAME_LENGTH];
} gg_asset_editor_t;

void AssetEditor_Create(gg_asset_editor_t* asset_editor);
void AssetEditor_AssetPairInfo(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_code_editor_t* code_editor,
                               gg_asset_pair_t* pair);
void AssetEditor_Do(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets,
                    gg_window_t* window);
void AssetEditor_Destroy(gg_asset_editor_t* asset_editor);

#endif  // GG_ASSET_EDITOR_H
