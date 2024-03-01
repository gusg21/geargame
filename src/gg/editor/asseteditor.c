#include "asseteditor.h"

#include "string.h"

#include "../assets.h"
#include "../cimgui_impl_raylib.h"
#include "editor.h"
#include "ggwidgets.h"

void AssetEditor_Create(gg_asset_editor_t* asset_editor) {
    memset(asset_editor->new_asset_name, 0, EDITOR_NEW_ASSET_NAME_LENGTH);
    asset_editor->open = true;
}

void AssetEditor_AssetPairInfo(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_code_editor_t* code_editor, gg_asset_pair_t* pair) {
    const char* type_name = Assets_GetTypeName(pair->asset.type);

    igLabelText("Name", "[ %s ]", pair->name);
    igLabelText("Type", "[ %d : %s ]", pair->asset.type, type_name);

    switch (pair->asset.type) {
        case ASSET_TEXTURE: {
            float ratio = 100.f / pair->asset.data.as_tex._handle.width;
            igImage(&pair->asset.data.as_tex._handle.id,
                    (ImVec2){100.f, pair->asset.data.as_tex._handle.height * ratio}, (ImVec2){0, 0}, (ImVec2){1, 1},
                    (ImVec4){1, 1, 1, 1}, (ImVec4){1, 1, 1, 1});
            break;
        }
        case ASSET_SCRIPT: {
            igValue_Bool("OK?", pair->asset.data.as_script.ok);
            igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){235 / 255.f, 136 / 255.f, 225 / 255.f, 255 / 255.f});
            if (igButton("Edit Code", (ImVec2){100, 20})) {
                CodeEditor_EditScriptAsset(code_editor, pair);
                editor->is_code_editor_visible = true;
            }
            igPopStyleColor(1);

            break;
        }
        case ASSET_ACTOR_SPEC: {
            igText("Actor Name: \"%s\"", pair->asset.data.as_actor_spec.name);
            igText("Script Asset: \"%s\"", pair->asset.data.as_actor_spec.script_asset_name);
            igText("Initial Position: (%.2f, %.2f)", pair->asset.data.as_actor_spec.initial_pos.x,
                   pair->asset.data.as_actor_spec.initial_pos.y);
            break;
        }
        default:
            igTextColored((ImVec4){0.5f, 0.5f, 0.5f, 1.0f}, "No preview for %s", type_name);
            break;
    }
}


void AssetEditor_Do(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets,
                    gg_window_t* window) {
    igBegin("Assets Viewer", &editor->is_assets_viewer_open, 0);
    {
        GGWidgets_Header(editor, (ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, "Assets Viewer");

        if (igBeginPopup("Create New Asset Popup", ImGuiWindowFlags_NoDecoration)) {
            if (igBeginPopup("Create New Actor Spec Popup", ImGuiWindowFlags_NoDecoration)) {
                GGWidgets_Header(editor, (ImVec4){225 / 255.f, 255 / 255.f, 74 / 255.f, 255 / 255.f}, "New Actor Spec");

                igInputText2("Asset Name", asset_editor->new_asset_name, EDITOR_NEW_ASSET_NAME_LENGTH);

                if (igButton2("Create!")) {
                    gg_asset_pair_t* new_asset_pair =
                        Assets_CreateNew(assets, ASSET_ACTOR_SPEC, asset_editor->new_asset_name);

                    igClosePopupToLevel(0, true);
                }

                igEndPopup();
            }

            for (uint32_t type = 0; type < ASSET_TYPE_COUNT; type++) {
                if (igButton2(Assets_GetTypeName(type))) {
                    switch (type) {
                        case ASSET_ACTOR_SPEC:
                            igOpenPopup_Str("Create New Actor Spec Popup", 0);
                            break;
                        default:
                            igCloseCurrentPopup();
                            break;
                    }
                }
            }

            igEndPopup();
        }

        if (igButton2("Create New Asset")) {
            igOpenPopup_Str("Create New Asset Popup", 0);
        }

        igSeparator();

        gg_asset_pair_t* pair = assets->asset_list;
        int id = 0;
        while (pair != NULL) {
            const char* type_name = Assets_GetTypeName(pair->asset.type);

            if (igTreeNode_StrStr(pair->name, "%s (%s)", pair->name, type_name)) {
                AssetEditor_AssetPairInfo(&editor->asset_editor, editor, &editor->code_editor, pair);

                igTreePop();
            }
            pair = pair->next;
            id++;
        }
    }
    igEnd();
}
