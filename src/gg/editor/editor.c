#ifdef GG_EDITOR

#include <string.h>

#include "../actor.h"
#include "../assets.h"
#include "editor.h"
#include "../state.h"
#include "../utils.h"

static gg_editor_t* g_editor;

static void Editor_S_AppendLuaConsoleLineColored(gg_editor_t* editor, const char* text, gg_color_t color) {
    char* new_line = calloc(EDITOR_LUA_MAX_LINE_LENGTH, sizeof(char));
    sprintf_s(new_line, EDITOR_LUA_MAX_LINE_LENGTH, "%s", text);

    free(editor->lua_console_lines[EDITOR_LUA_CONSOLE_LINES - 1]);  // Clear the last one

    for (size_t i = EDITOR_LUA_CONSOLE_LINES - 1; i > 0; i--) {
        editor->lua_console_lines[i] = editor->lua_console_lines[i - 1];
        editor->lua_console_line_colors[i] = editor->lua_console_line_colors[i - 1];
    }

    editor->lua_console_lines[0] = new_line;
    editor->lua_console_line_colors[0] = color;
}

static void Editor_S_AppendLuaConsoleLine(gg_editor_t* editor, const char* text) {
    Editor_S_AppendLuaConsoleLineColored(editor, text, COL(255, 255, 255));
}

static void Editor_S_TraceLogCallback(int logLevel, const char* text, va_list args) {
    char line[1024] = {0};
    sprintf_s(line, 1024, "%d: %s", logLevel, text);
    printf("%s\n", line);
}

static int Editor_S_LuaPrint(lua_State* L) {
    const char* text = lua_tostring(L, 1);

    printf("%s\t", text);
    Editor_S_AppendLuaConsoleLine(g_editor, text);
    printf("\n");

    return 0;
}

void Editor_Create(gg_editor_t* editor, gg_scripting_t* scripting) {
    g_editor = editor;

    TextEditor_create(&editor->text_editor);

    editor->is_scene_viewer_visible = true;
    editor->is_assets_viewer_visible = true;
    editor->lua_console_lines = calloc(EDITOR_LUA_CONSOLE_LINES, sizeof(char*));

    SetTraceLogCallback(Editor_S_TraceLogCallback);
    lua_register(scripting->state, "print", Editor_S_LuaPrint);
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

static void Editor_S_Actor(gg_editor_t* editor, gg_actor_t* actor, uint32_t actor_id, char** script_names,
                           gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    // Quick info
    igText("ID# %d Alive: %d", actor->actor_id, actor->alive);

    igSeparator();

    // Script choosing
    gg_scripting_t* script = &state->current_scene->scripting;
    igPushID_Int(actor_id << 8);
    if (igBeginCombo("Script", "Choose a script", 0)) {
        for (uint32_t i = 0; i < 64; i++) {
            if (script_names[i] == NULL) {
                break;
            }

            if (igButton2(script_names[i])) {
                gg_asset_t* asset = NULL;
                bool valid = Assets_Get(assets, &asset, script_names[i]);
                if (valid) {
                    Scripting_ReloadScript(script, &asset->data.as_script, actor->script_handle);
                    Scripting_SetPointerBouquet(script, actor->script_handle, actor, state, window, assets);
                    Scripting_Call(script, "ready", actor->script_handle);
                }
            }
        }

        igEndCombo();
    }
    igPopID();
    igText("Handle# %d", actor->script_handle);
    igSameLine2();
    if (igButton2("Copy Handle String")) {
        char handle_str[16] = {0};
        Scripting_GetHandleString(script, handle_str, 15, actor->script_handle);
        SetClipboardText(handle_str);
    }
    igSameLine2();
    if (igButton2("Ready")) {
        Scripting_Call(script, "ready", actor->script_handle);
    }
    igSeparator();

    // Transform
    igInputFloat2_s("Position", (float*)&actor->transform.pos);
    igInputFloat2_s("Scale", (float*)&actor->transform.scale);
    igInputFloat_s("Rotation", &actor->transform.rot);
    igSeparator();

    // Misc
    igInputText2("Name", actor->name, 31);
    igCheckbox("Visible", &actor->visible);
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

static void Editor_S_ToggleButton(const char* label, bool* value) {
    bool highlight = *value;
    if (highlight) igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){92 / 255.f, 176 / 255.f, 214 / 255.f, 255 / 255.f});
    if (igButton2(label)) {
        *value = !*value;
    }
    if (highlight) igPopStyleColor(1);
}

static void Editor_S_DoSceneViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImGuiIO* io = igGetIO();
    igBegin("Scene Viewer", &editor->is_scene_viewer_open, 0);
    {
        igSeparator();

        Editor_S_Header(editor, (ImVec4){235 / 255.f, 185 / 255.f, 72 / 255.f, 255 / 255.f}, "Scene Actors");

        const uint32_t max_script_names = 64;
        char** script_names = calloc(max_script_names, sizeof(char*));
        Assets_FindAssetsByType(assets, script_names, ASSET_SCRIPT);

        // For each actor
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
                    Editor_S_Actor(editor, actor, actor_id, script_names, state, assets, window);
                    igUnindent2();
                }
                igPopID();
                igSpacing();
            }
        }

        free(script_names);
    }
    igEnd();
}

static void Editor_S_DoAssetsViewer(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    igBegin("Assets Viewer", &editor->is_assets_viewer_open, 0);
    {
        Editor_S_Header(editor, (ImVec4){22 / 255.f, 247 / 255.f, 109 / 255.f, 255 / 255.f}, "Assets Viewer");

        if (igBeginPopup("Create New Asset Popup", ImGuiWindowFlags_NoDecoration)) {
            if (igBeginPopup("Create New Actor Spec Popup", ImGuiWindowFlags_NoDecoration)) {
                Editor_S_Header(editor, (ImVec4){225/255.f, 255/255.f, 74/255.f, 255/255.f}, "New Actor Spec");

                igInputText2("Asset Name", editor->new_asset_name, EDITOR_NEW_ASSET_NAME_LENGTH);

                if (igButton2("Create!")) {
                    gg_asset_pair_t* new_asset_pair = Assets_CreateNew(assets, ASSET_ACTOR_SPEC, editor->new_asset_name);

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
    if (pair->asset.data.as_script.text != NULL) free(pair->asset.data.as_script.text);

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

static void Editor_S_DoToolbar(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    bool open = true;

    igBegin("##Toolbar", &open,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        igSetWindowPos_Vec2((ImVec2){0, 0}, ImGuiCond_Always);
        igSetWindowSize_Vec2((ImVec2){(float)GetScreenWidth(), 36}, ImGuiCond_Always);

        if (igButton2("Quit")) {
            State_Quit(state);
        }
        igSameLine2();

        igText("gg engine");
        igSameLine2();

        igText("-");
        igSameLine2();

        Editor_S_ToggleButton("Scene", &editor->is_scene_viewer_visible);
        igSameLine2();

        Editor_S_ToggleButton("Code", &editor->is_code_editor_visible);
        igSameLine2();

        Editor_S_ToggleButton("Assets", &editor->is_assets_viewer_visible);
        igSameLine2();

        Editor_S_ToggleButton("Lua", &editor->is_lua_console_visible);
        igSameLine2();

        Editor_S_ToggleButton("ImGui Demo", &editor->is_demo_window_visible);
        igSameLine2();

        igText("FPS %.2f (ft %.2f)", 1.f / Window_GetDeltaTime(window), Window_GetDeltaTime(window));
    }
    igEnd();
}

static void Editor_S_ExecuteLuaLine(gg_editor_t* editor, gg_scripting_t* script) {
    Editor_S_AppendLuaConsoleLine(editor, TextFormat("> %s", editor->lua_input_line));

    int32_t error = Scripting_RunCode(script, editor->lua_input_line);
    if (error > 0) {
        Editor_S_AppendLuaConsoleLineColored(editor, TextFormat("%s", Scripting_GetError(script)), COL(255, 50, 50));
        Editor_S_AppendLuaConsoleLineColored(editor, TextFormat("err %d", error), COL(120, 0, 0));
    }

    memset(editor->lua_input_line, 0, EDITOR_LUA_INPUT_LINE_LENGTH);
}

static void Editor_S_DoLuaConsole(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    ImVec2 size;
    igCalcTextSize(&size, "TALL LETTERS", NULL, false, 0);
    float height = igGetStyle()->ItemSpacing.y + size.y;
    igSetNextWindowSizeConstraints2((ImVec2){0.f, 0.f}, (ImVec2){FLT_MAX, (EDITOR_LUA_CONSOLE_LINES + 4) * height});
    if (igBegin("Lua Console", &editor->is_lua_console_open, 0)) {
        igBeginChild_ID(1791, (ImVec2){0, -30}, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        for (int32_t i = EDITOR_LUA_CONSOLE_LINES - 1; i >= 0; i--) {
            if (editor->lua_console_lines[i]) {
                igTextColored(
                    (ImVec4){editor->lua_console_line_colors[i].r / 255.f, editor->lua_console_line_colors[i].g / 255.f,
                             editor->lua_console_line_colors[i].b / 255.f,
                             editor->lua_console_line_colors[i].a / 255.f},
                    "%s", editor->lua_console_lines[i]);
            } else {
                igText("");
            }
        }
        igEndChild();

        if (igInputText("<", editor->lua_input_line, 127, ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL)) {
            igSetKeyboardFocusHere(-1);
            Editor_S_ExecuteLuaLine(editor, &state->current_scene->scripting);
        }
        igSameLine2();
        if (igButton2("Enter")) {
            Editor_S_ExecuteLuaLine(editor, &state->current_scene->scripting);
        }
    }
    igEnd();
}

void Editor_Update(gg_editor_t* editor, gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_S_DoToolbar(editor, state, assets, window);

    if (editor->is_scene_viewer_visible) Editor_S_DoSceneViewer(editor, state, assets, window);
    if (editor->is_assets_viewer_visible) Editor_S_DoAssetsViewer(editor, state, assets, window);
    if (editor->is_code_editor_visible) Editor_S_DoCodeEditor(editor, state, assets, window);
    if (editor->is_lua_console_visible) Editor_S_DoLuaConsole(editor, state, assets, window);
    if (editor->is_demo_window_visible) igShowDemoWindow(&editor->is_demo_window_open);
}

#endif  // GG_EDITOR