#if !defined(GG_WIDGETS_H)
#define GG_WIDGETS_H

#include <stdio.h>
#include <stdlib.h>

#include "../color.h"
#include "../cimgui_impl_raylib.h"

#define WIDGETS_CONSOLE_MAX_LINE_LEN 512

typedef struct gg_actor gg_actor_t;
typedef struct gg_state gg_state_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_window gg_window_t;

typedef struct gg_console {
    size_t line_count;
    char** lines;
    gg_color_t* line_colors;
    bool has_new_line;
    uint32_t my_id;
} gg_console_t;

void GGWidgets_Console_Create(gg_console_t* console, size_t line_count);
void GGWidgets_Console_AppendLineColored(gg_console_t* console, const char* text, gg_color_t color);
void GGWidgets_Console_AppendLine(gg_console_t* console, const char* text);
void GGWidgets_Console_Do(gg_console_t* console, bool space_for_input);
void GGWidgets_Console_Destroy(gg_console_t* console);

void GGWidgets_Header(ImVec4 color, const char* text);
void GGWidgets_ToggleButton(const char* label, bool* value);
void GGWidgets_Actor(gg_actor_t* actor, uint32_t actor_id, char** script_names, gg_state_t* state, gg_assets_t* assets,
                     gg_window_t* window);

#endif  // GG_WIDGETS_H
