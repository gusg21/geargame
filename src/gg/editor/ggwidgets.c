#include "ggwidgets.h"

#include "../actor.h"
#include "../state.h"
#include "../utils.h"
#include "editor.h"

void GGWidgets_Console_Create(gg_console_t* console, size_t line_count) {
    console->lines = calloc(line_count, sizeof(char*));
    console->line_colors = calloc(line_count, sizeof(gg_color_t));
    console->line_count = line_count;
}

void GGWidgets_Console_AppendLineColored(gg_console_t* console, const char* text, gg_color_t color) {
    char* new_line = calloc(WIDGETS_CONSOLE_MAX_LINE_LEN, sizeof(char));
    sprintf_s(new_line, WIDGETS_CONSOLE_MAX_LINE_LEN, "%s", text);

    free(console->lines[console->line_count - 1]);  // Clear the last one

    for (size_t i = console->line_count - 1; i > 0; i--) {
        console->lines[i] = console->lines[i - 1];
        console->line_colors[i] = console->line_colors[i - 1];
    }

    console->lines[0] = new_line;
    console->line_colors[0] = color;
}

void GGWidgets_Console_AppendLine(gg_console_t* console, const char* text) {
    GGWidgets_Console_AppendLineColored(console, text, COL(255, 255, 255));
}

void GGWidgets_Console_Do(gg_console_t* console, bool space_for_input) {
    igBeginChild_ID(1791, (ImVec2){0, space_for_input ? -30 : 0}, ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_HorizontalScrollbar);
    for (int32_t i = console->line_count - 1; i >= 0; i--) {
        if (console->lines[i] != NULL) {
            igTextColored((ImVec4){console->line_colors[i].r / 255.f, console->line_colors[i].g / 255.f,
                                   console->line_colors[i].b / 255.f, console->line_colors[i].a / 255.f},
                          "%s", console->lines[i]);
        } else {
            igText("");
        }
    }
    igEndChild();
}

void GGWidgets_Header(ImVec4 color, const char* text) {
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

void GGWidgets_ToggleButton(const char* label, bool* value) {
    bool highlight = *value;
    if (highlight) igPushStyleColor_Vec4(ImGuiCol_Button, (ImVec4){92 / 255.f, 176 / 255.f, 214 / 255.f, 255 / 255.f});
    if (igButton2(label)) {
        *value = !*value;
    }
    if (highlight) igPopStyleColor(1);
}

void GGWidgets_Actor(gg_actor_t* actor, uint32_t actor_id, char** script_names, gg_state_t* state, gg_assets_t* assets,
                     gg_window_t* window) {
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
