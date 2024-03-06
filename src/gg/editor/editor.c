#ifdef GG_EDITOR

#include <string.h>

#include "../actor.h"
#include "../assets.h"
#include "../external/icons.h"
#include "../state.h"
#include "../utils.h"
#include "editor.h"
#include "ggwidgets.h"
#include "luaconsole.h"

static gg_editor_t* g_editor;

void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting) {
    g_editor = editor;

    AssetEditor_Create(&editor->asset_editor);
    CodeEditor_Create(&editor->code_editor);
    LuaConsole_Create(&editor->lua_console, scripting);
    OutputConsole_Create(&editor->output_console);

    editor->is_scene_viewer_visible = true;
    editor->is_assets_viewer_visible = true;
}

static void Editor_S_DoSceneViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImGuiIO* io = igGetIO();
    igBegin(ICON_GG_SCENE " Scene Viewer", &editor->is_scene_viewer_open, 0);
    {
        GGWidgets_ToggleButton(state->current_scene->paused ? ICON_GG_PLAY : ICON_GG_PAUSE, &state->current_scene->paused);

        igSeparator();

        GGWidgets_Header((ImVec4){235 / 255.f, 185 / 255.f, 72 / 255.f, 255 / 255.f}, ICON_GG_SCENE " Scene Viewer");

        // Spawning/controls
        const uint32_t max_actor_spec_names = 64;
        char* actor_spec_names[max_actor_spec_names] = {0};
        Assets_FindAssetsByType(assets, actor_spec_names, ASSET_ACTOR_SPEC);

        if (igBeginCombo("Actor Spec", ICON_GG_ACTOR_SPEC " Spawn an Actor Spec...", 0)) {
            for (size_t i = 0; i < max_actor_spec_names; i++) {
                if (actor_spec_names[i] != NULL) {
                    const char* name = actor_spec_names[i];
                    if (igButton2(name)) {
                        gg_asset_t* spec_asset;
                        bool valid = Assets_Get(assets, &spec_asset, name);
                        if (valid) {
                            Scene_NewActorFromSpec(state->current_scene, assets, window,
                                                   &spec_asset->data.as_actor_spec);
                        }
                        igClosePopupToLevel(0, true);
                    }
                }
            }
            igEndCombo();
        }
        igSeparator();

        // Show actors
        const uint32_t max_script_names = 64;
        char* script_names[max_script_names] = {0};
        Assets_FindAssetsByType(assets, script_names, ASSET_SCRIPT);

        gg_scene_t* scene = state->current_scene;
        for (uint32_t actor_id = 0; actor_id < SCENE_MAX_ACTORS; actor_id++) {
            gg_actor_t* actor = &scene->actors[actor_id];
            if (actor->alive) {
                igText("[%d] %s", actor_id, actor->name);
                igSameLine2();
                igSetCursorPosX(140);
                igPushID_Int(actor_id);
                if (igCollapsingHeader_Str("Details")) {
                    igIndent2();
                    GGWidgets_Actor(actor, actor_id, script_names, state, assets, window);
                    igUnindent2();
                }
                igPopID();
                igSpacing();
            }
        }
    }
    igEnd();
}

static void Editor_S_DoToolbar(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    bool open = true;

    igBegin("##Toolbar", &open,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        igSetWindowPos_Vec2((ImVec2){0, 0}, ImGuiCond_Always);
        igSetWindowSize_Vec2((ImVec2){(float)GetScreenWidth(), 36}, ImGuiCond_Always);

        if (igButton2(ICON_GG_CLOSE " Quit")) {
            State_Quit(state);
        }
        igSameLine2();

        igCheckbox("Mega", &state->render_to_window);
        igSameLine2();

        igText(ICON_GG_GAME " gg engine");
        igSameLine2();

        igText("-");
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_SCENE " Scene", &editor->is_scene_viewer_visible);
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_SCRIPT " Code", &editor->is_code_editor_visible);
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_ASSET " Assets", &editor->is_assets_viewer_visible);
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_LUA " Lua", &editor->is_lua_console_visible);
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_OUTPUT " Output", &editor->is_output_console_visible);
        igSameLine2();

        GGWidgets_ToggleButton(ICON_GG_IMGUI " ImGui Demo", &editor->is_demo_window_visible);
        igSameLine2();

        igText("FPS %.2f (ft %.2f)", 1.f / Window_GetDeltaTime(window), Window_GetDeltaTime(window));
    }
    igEnd();
}

static void Editor_S_DoGameView(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igSetNextWindowSize((ImVec2){window->_tex.texture.width / 2, window->_tex.texture.height / 2},
                        ImGuiCond_FirstUseEver);
    if (igBegin(ICON_GG_GAME " Game", &editor->is_game_open, ImGuiWindowFlags_MenuBar)) {
        ImVec2 avail_size, window_size;
        igGetContentRegionAvail(&avail_size);
        igGetWindowSize(&window_size);

        bool correct = false;
        if (igBeginMenuBar()) {
            if (igButton2("Correct Aspect")) {
                correct = true;
            }
            igEndMenuBar();
        }

        if (correct) {
            igSetWindowSize_Vec2(
                (ImVec2){window_size.x, window_size.x * ((float)window->_tex.texture.height / (float)window->_tex.texture.width)},
                ImGuiCond_Always);
        }

        // Draw the game
        igImage(&window->_tex.texture.id, avail_size, (ImVec2){0, 1}, (ImVec2){1, 0}, (ImVec4){1.f, 1.f, 1.f, 1.f}, (ImVec4){1.f, 1.f, 1.f, 1.f});
    }
    igEnd();
}

void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_S_DoToolbar(editor, state, assets, window);

    // igDockSpaceOverViewport2(igGetMainViewport());

    if (editor->is_scene_viewer_visible) Editor_S_DoSceneViewer(editor, state, assets, window);
    if (editor->is_assets_viewer_visible) AssetEditor_Do(&editor->asset_editor, editor, state, assets, window);
    if (editor->is_code_editor_visible) CodeEditor_Do(&editor->code_editor);
    if (editor->is_lua_console_visible) LuaConsole_Do(&editor->lua_console, &state->current_scene->scripting);
    if (editor->is_output_console_visible) OutputConsole_Do(&editor->output_console);
    if (editor->is_demo_window_visible) igShowDemoWindow(&editor->is_demo_window_open);
    if (!state->render_to_window) Editor_S_DoGameView(editor, state, assets, window);    
}

void Editor_Destroy(gg_editor_t* editor) {
    CodeEditor_Destroy(&editor->code_editor);
    AssetEditor_Destroy(&editor->asset_editor);
    LuaConsole_Destroy(&editor->lua_console);
    OutputConsole_Destroy(&editor->output_console);
}

#endif  // GG_EDITOR