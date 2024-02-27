#ifdef GG_DEBUG

#include <string.h>

#include "actor.h"
#include "assets.h"
#include "editor.h"
#include "state.h"

void Editor_Create(gg_editor_t* editor) { TextEditor_create(&editor->text_editor); }

static void Editor_S_Header(gg_editor_t* editor, ImVec4 color, const char* text) {
    // float width = igGetWindowWidth();
    // ImVec2 size = {0};
    // igCalcTextSize(&size, text, NULL, false, 1000.f);

    // igSetCursorPosX((width - size.x) / 2.f);
    // igTextColored(color, text);

    ImGuiStyle* style = igGetStyle();
    ImVec4 old_color = style->Colors[ImGuiCol_Text];
    style->Colors[ImGuiCol_Text] = color;
    igSeparatorText(text);
    style->Colors[ImGuiCol_Text] = old_color;

    igSeparator();
}

static void Editor_S_Actor(gg_editor_t* editor, gg_actor_t* actor, uint32_t actor_id, char** script_names,
                           gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igPushID_Int(actor_id);
    if (igBeginCombo("Script", "Choose a script", 0)) {
        for (uint32_t i = 0; i < 64; i++) {
            if (script_names[i] == NULL) {
                break;
            }

            if (igButton2(script_names[i])) {
                gg_asset_t* asset = NULL;
                bool valid = Assets_Get(assets, &asset, script_names[i]);
                if (valid) {
                    gg_scripting_t* script = &state->current_scene->scripting;
                    Scripting_ReloadScript(script, &asset->data.as_script, actor->script_handle);
                    Scripting_CallWithPointerBouquet(script, "initialize", actor->script_handle, actor, state, window,
                                                     assets);
                    Scripting_Call(script, "ready", actor->script_handle);
                }
            }
        }

        igEndCombo();
    }
    igPopID();
}

static void Editor_S_DoStateViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImGuiIO* io = igGetIO();
    igBegin("State Viewer", &editor->is_state_viewer_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){235 / 255.f, 85 / 255.f, 52 / 255.f, 255 / 255.f}, "State Viewer");
        igText("FPS %.2f (ft %.2f)", 1.f / Window_GetDeltaTime(window), Window_GetDeltaTime(window));
        igText("shift [%d] alt [%d] ctrl [%d] super [%d]", io->KeyShift, io->KeyAlt, io->KeyCtrl, io->KeySuper);

        igSeparator();

        if (igButton2("Quit")) {
            State_Quit(state);
        }

        Editor_S_Header(editor, (ImVec4){235 / 255.f, 185 / 255.f, 72 / 255.f, 255 / 255.f}, "Scene Actors");

        const uint32_t max_script_names = 64;
        char** script_names = calloc(max_script_names, sizeof(char*));
        Assets_FindAssetsByType(assets, script_names, ASSET_SCRIPT);

        // For each actor
        gg_scene_t* scene = state->current_scene;
        for (uint32_t actor_id = 0; actor_id < SCENE_MAX_ACTORS; actor_id++) {
            gg_actor_t* actor = &scene->actors[actor_id];
            if (actor->alive) {
                if (igCollapsingHeader_Str(TextFormat("[%d] \"%s\"", actor_id, actor->name))) {
                    Editor_S_Actor(editor, actor, actor_id, script_names, state, assets, window);
                }
            }
        }

        free(script_names);
    }
    igEnd();
}

static void Editor_S_EditScriptAsset(gg_editor_t* editor, gg_asset_pair_t* pair) {
    TextEditor_setText(&editor->text_editor, pair->asset.data.as_script.text);
    editor->text_editor_pair = pair;
}

static void Editor_S_AssetPairInfo(gg_editor_t* editor, gg_asset_pair_t* pair) {
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
                Editor_S_EditScriptAsset(editor, pair);
            }
            igPopStyleColor(1);

            break;
        }
        default:
            igTextColored((ImVec4){0.5f, 0.5f, 0.5f, 1.0f}, "No preview for %s", type_name);
            break;
    }
}

static void Editor_S_DoAssetsViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("Assets Viewer", &editor->is_assets_viewer_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, "Assets Viewer");

        gg_asset_pair_t* pair = assets->asset_list;
        int id = 0;
        while (pair != NULL) {
            const char* type_name = Assets_GetTypeName(pair->asset.type);

            if (igTreeNode_StrStr(pair->name, "%s (%s)", pair->name, type_name)) {
                Editor_S_AssetPairInfo(editor, pair);

                igTreePop();
            }
            pair = pair->next;
            id++;
        }
    }
    igEnd();
}

static void Editor_S_SaveTextEditorAsset(gg_editor_t* editor, gg_asset_pair_t* pair) {
    char* code = TextEditor_getText(&editor->text_editor);  // Managed by us now
    pair->asset.data.as_script.text = code;
    pair->asset.data.as_script.ok = true;
}

static void Editor_S_DoCodeEditor(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("Code Editor", &editor->is_code_editor_open, 0);
    {
        if (editor->text_editor_pair != NULL) {
            igText("Editing asset %s", editor->text_editor_pair->name);

            igSameLine2();

            if (igButton("Save Asset", (ImVec2){100, 17})) {
                Editor_S_SaveTextEditorAsset(editor, editor->text_editor_pair);
            }
        } else {
            igText("Pick an asset from the Assets Viewer to edit its code");
        }

        TextEditor_render(&editor->text_editor);
    }
    igEnd();
}

void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_S_DoStateViewer(editor, state, assets, window);
    Editor_S_DoAssetsViewer(editor, state, assets, window);
    Editor_S_DoCodeEditor(editor, state, assets, window);

    igShowDemoWindow(&editor->is_demo_window_open);
}

#endif  // GG_DEBUG