#ifdef GG_EDITOR

#include "asseteditor.h"

#include "string.h"

#include "../assets.h"
#include "../cimgui_impl_raylib.h"
#include "../state.h"
#include "../utils.h"
#include "editor.h"
#include "ggwidgets.h"

void AssetEditor_Create(gg_asset_editor_t* asset_editor) {
    memset(asset_editor->new_asset_name, 0, EDITOR_NEW_ASSET_NAME_LENGTH);
}

void AssetEditor_AssetPairInfo(gg_asset_editor_t* asset_editor, gg_assets_t* assets, gg_editor_t* editor,
                               gg_code_editor_t* code_editor, gg_state_t* state, gg_window_t* window,
                               gg_asset_pair_t* pair) {
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
            igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){205 / 255.f, 106 / 255.f, 195 / 255.f, 255 / 255.f});
            if (igButton2(ICON_GG_EDIT "Edit Code")) {
                CodeEditor_EditScriptAsset(code_editor, pair);
                editor->is_code_editor_visible = true;
            }
            igPopStyleColor(1);

            break;
        }
        case ASSET_ACTOR_SPEC: {
            igText("Actor Name: \"%s\"", pair->asset.data.as_actor_spec.name);
            igInputText2("Script Name", pair->asset.data.as_actor_spec.script_asset_name,
                         ACTOR_SPEC_MAX_SCRIPT_NAME_LEN);
            igText("Initial Position: (%.2f, %.2f)", pair->asset.data.as_actor_spec.initial_pos.x,
                   pair->asset.data.as_actor_spec.initial_pos.y);
            break;
        }
        case ASSET_SCENE: {
            if (pair->asset.data.as_scene.paused) {
                igText(ICON_GG_PAUSE " Paused");
            } else {
                igText(ICON_GG_PLAY " Unpaused");
            }

            bool is_current = &pair->asset.data.as_scene == state->current_scene;
            igBeginDisabled(is_current);
            if (igButton2(ICON_GG_EDIT " Edit Scene")) {
                State_SetCurrentScene(state, &pair->asset.data.as_scene);
            }
            igEndDisabled();
            if (is_current) {
                igSameLine2();
                igText(ICON_GG_CLOSE " Scene already open");
            }
            break;
        }
        default:
            igTextColored((ImVec4){0.5f, 0.5f, 0.5f, 1.0f}, "No preview for %s", type_name);
            break;
    }

    igSeparator();

    igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){80 / 255.f, 156 / 255.f, 115 / 255.f, 255 / 255.f});
    ImVec2 size = {0};
    igGetContentRegionAvail(&size);
    igPushItemWidth(size.x);
    if (igButton(ICON_GG_SAVE "Save Asset", (ImVec2){size.x, 0.f})) {
        Assets_SaveAssetPair(assets, pair);
    }
    igPopItemWidth();
    igPopStyleColor(1);
}

static void AssetEditor_S_DoCreatePopups(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_state_t* state,
                                         gg_assets_t* assets, gg_window_t* window) {
    if (igBeginPopup("Create New Actor Spec Popup", ImGuiWindowFlags_NoDecoration)) {
        GGWidgets_Header((ImVec4){225 / 255.f, 255 / 255.f, 74 / 255.f, 255 / 255.f},
                         ICON_GG_ACTOR_SPEC " New Actor Spec");

        igInputText2("Actor Spec Name", asset_editor->new_asset_name, EDITOR_NEW_ASSET_NAME_LENGTH);

        if (igButton2("Create!")) {
            gg_asset_pair_t* new_asset_pair = Assets_CreateNew(assets, ASSET_ACTOR_SPEC, asset_editor->new_asset_name);

            igClosePopupToLevel(0, true);
        }

        igEndPopup();
    }

    if (igBeginPopup("Create New Script Popup", ImGuiWindowFlags_NoDecoration)) {
        GGWidgets_Header((ImVec4){225 / 255.f, 255 / 255.f, 74 / 255.f, 255 / 255.f}, ICON_GG_SCRIPT " New Script");

        igInputText2("Script Name", asset_editor->new_asset_name, EDITOR_NEW_ASSET_NAME_LENGTH);

        if (igButton2("Create!")) {
            gg_asset_pair_t* new_asset_pair = Assets_CreateNew(assets, ASSET_SCRIPT, asset_editor->new_asset_name);
            Script_CreateEmpty(&new_asset_pair->asset.data.as_script);

            igClosePopupToLevel(0, true);
        }

        igEndPopup();
    }

    if (igBeginPopup("Create New Scene Popup", ImGuiWindowFlags_NoDecoration)) {
        GGWidgets_Header((ImVec4){225 / 255.f, 255 / 255.f, 74 / 255.f, 255 / 255.f}, ICON_GG_SCENE " New Scene");

        igInputText2("Script Name", asset_editor->new_asset_name, EDITOR_NEW_ASSET_NAME_LENGTH);

        if (igButton2("Create!")) {
            gg_asset_pair_t* new_asset_pair = Assets_CreateNew(assets, ASSET_SCENE, asset_editor->new_asset_name);
            Scene_Create(&new_asset_pair->asset.data.as_scene, window, state);
            new_asset_pair->asset.data.as_scene.paused = true;

            igClosePopupToLevel(0, true);
        }

        igEndPopup();
    }
}

static const char* AssetEditor_S_GetTypeIcon(gg_asset_type_e type) {
    switch (type) {
        case ASSET_ACTOR_SPEC:
            return ICON_GG_ACTOR_SPEC;
        case ASSET_SCENE:
            return ICON_GG_SCENE;
        case ASSET_SCRIPT:
            return ICON_GG_SCRIPT;
        case ASSET_TEXTURE:
            return ICON_GG_TEXTURE;
        case ASSET_TILED_MAP:
            return ICON_GG_TILED_MAP;
        default:
            return ICON_GG_UNKNOWN;
    }
}

void AssetEditor_Do(gg_asset_editor_t* asset_editor, gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets,
                    gg_window_t* window) {
    if (igBegin(ICON_GG_ASSET " Assets Viewer", NULL, 0)) {
        GGWidgets_Header((ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, ICON_GG_ASSET " Assets Viewer");

        if (igBeginPopup("Create New Asset Popup", ImGuiWindowFlags_NoDecoration)) {
            AssetEditor_S_DoCreatePopups(asset_editor, editor, state, assets, window);

            for (uint32_t type = ASSET_NONE + 1; type < ASSET_TYPE_COUNT; type++) {
                char buffer[64] = {0};
                sprintf_s(buffer, 64, "%s %s", AssetEditor_S_GetTypeIcon(type), Assets_GetTypeName(type));

                if (igButton2(buffer)) {
                    switch (type) {
                        case ASSET_ACTOR_SPEC:
                            igOpenPopup_Str("Create New Actor Spec Popup", 0);
                            break;
                        case ASSET_SCRIPT:
                            igOpenPopup_Str("Create New Script Popup", 0);
                            break;
                        case ASSET_SCENE:
                            igOpenPopup_Str("Create New Scene Popup", 0);
                            break;
                        default:
                            igCloseCurrentPopup();
                            break;
                    }
                }
            }

            igEndPopup();
        }

        if (igButton2(ICON_GG_ADD " Create New Asset")) {
            igOpenPopup_Str("Create New Asset Popup", 0);
        }

        igSeparator();

        gg_asset_pair_t* pair = assets->asset_list;
        int id = 0;
        while (pair != NULL) {
            const char* type_name = Assets_GetTypeName(pair->asset.type);
            const char* type_icon = AssetEditor_S_GetTypeIcon(pair->asset.type);
            bool is_current_scene = false;
            if (pair->asset.type == ASSET_SCENE && state->current_scene == &pair->asset.data.as_scene) {
                is_current_scene = true;
            }

            if (igTreeNode_StrStr(pair->name, "%s %s (%s) %s", type_icon, pair->name, type_name,
                                  is_current_scene ? ICON_GG_SPECIAL : "")) {
                AssetEditor_AssetPairInfo(&editor->asset_editor, assets, editor, &editor->code_editor, state, window, pair);

                igTreePop();
            }
            pair = pair->next;
            id++;
        }
    }
    igEnd();
}

void AssetEditor_Destroy(gg_asset_editor_t* asset_editor) {
    // Don't need to clean up anything
}

#endif
