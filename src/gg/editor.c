#include "editor.h"
#include "cimgui_impl_raylib.h"
#include "state.h"
#include "assets.h"

void Editor_Create(gg_editor_t* editor) { editor->is_editor_open = false; }

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
    }
    igEnd();
}

static void Editor_S_DoAssetsViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("Assets Viewer", &editor->is_editor_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, "Assets Viewer");

        gg_asset_pair_t* pair = assets->asset_list;
        while (pair != NULL) {
            if (igTreeNode_Str(pair->name))
            {
                igLabelText("Name", "[ %s ]", pair->name);
                igLabelText("Type", "[ %d ]", pair->asset.type);
                igTreePop();
            }
            pair = pair->next;
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