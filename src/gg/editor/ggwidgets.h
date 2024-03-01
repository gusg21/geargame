#include "../actor.h"
#include "../state.h"
#include "editor.h"

static void GGWidgets_Header(gg_editor_t* editor, ImVec4 color, const char* text) {
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

static void GGWidgets_Actor(gg_editor_t* editor, gg_actor_t* actor, uint32_t actor_id, char** script_names,
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
