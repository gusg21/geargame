#ifndef CIMGUI_RAYLIB_H
#define CIMGUI_RAYLIB_H

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <float.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdlib.h>
#include "cimgui/cimgui.h"
#include "cimgui/imgui/imconfig.h"

static double g_Time = 0.0;
static Texture2D g_FontTex;

bool ImGui_ImplRaylib_Init();
void ImGui_ImplRaylib_Shutdown();
void ImGui_ImplRaylib_NewFrame();
bool ImGui_ImplRaylib_ProcessEvent();
void ImGui_ImplRaylib_RenderDrawData(ImDrawData *draw_data);
void ImGui_ImplRaylib_Render();

#endif