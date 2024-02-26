#ifdef DEBUG

#include "editor.h"
#include "assets.h"
#include "state.h"

void Editor_Create(gg_editor_t* editor) {
    editor->is_editor_open = false;
    TextEditor_create(&editor->text);
}

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

static void Editor_S_DoStateViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("State Viewer", &editor->is_editor_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){235 / 255.f, 85 / 255.f, 52 / 255.f, 255 / 255.f}, "State Viewer");
        TextEditor_render(&editor->text);
    }
    igEnd();
}

static void Editor_S_AssetPairInfo(gg_asset_pair_t* pair) {
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
            if (igTreeNode_StrStr(pair->name, "Code")) {
                igText(pair->asset.data.as_script.text);

                igTreePop();
            }
            break;
        }
        default:
            igTextColored((ImVec4){0.5f, 0.5f, 0.5f, 1.0f}, "No preview for %s", type_name);
            break;
    }
}

static void Editor_S_DoAssetsViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("Assets Viewer", &editor->is_editor_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, "Assets Viewer");

        gg_asset_pair_t* pair = assets->asset_list;
        int id = 0;
        while (pair != NULL) {
            const char* type_name = Assets_GetTypeName(pair->asset.type);

            if (igTreeNode_StrStr(type_name, "%s (%s)", pair->name, type_name)) {
                Editor_S_AssetPairInfo(pair);

                igTreePop();
            }
            pair = pair->next;
            id++;
        }
    }
    igEnd();
}

void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_S_DoStateViewer(editor, state, assets, window);
    Editor_S_DoAssetsViewer(editor, state, assets, window);

    bool yeah;
    igShowDemoWindow(&yeah);
}

#endif  // DEBUG