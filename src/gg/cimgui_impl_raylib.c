#include "cimgui_impl_raylib.h"

#include <math.h>

bool wasShiftDown = false;
bool wasCtrlDown = false;
bool wasSuperDown = false;
bool wasAltDown = false;

static const char* ImGui_ImplRaylib_GetClipboardText(void* _) { return GetClipboardText(); }

static void ImGui_ImplRaylib_SetClipboardText(void* _, const char* text) { SetClipboardText(text); }

bool ImGui_ImplRaylib_Init() {
    rlEnableScissorTest();
    struct ImGuiIO* io = igGetIO();

    ImFontAtlas_AddFontDefault(io->Fonts, NULL);

    unsigned char* data;
    int width, height, bpp;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &data, &width, &height, &bpp);

    Image img = {
        .data = data, .width = width, .height = height, .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, .mipmaps = 1};
    g_FontTex = LoadTextureFromImage(img);

    ImFontAtlas_SetTexID(io->Fonts, &g_FontTex.id);

    io->BackendPlatformName = "cimgui_impl_raylib";

    io->MousePos = (ImVec2){-FLT_MAX, -FLT_MAX};
    io->SetClipboardTextFn = ImGui_ImplRaylib_SetClipboardText;
    io->GetClipboardTextFn = ImGui_ImplRaylib_GetClipboardText;
    io->ClipboardUserData = NULL;

    return true;
}

void ImGui_ImplRaylib_Shutdown() { g_Time = 0.0; }

static void ImGui_ImplRaylib_UpdateMouseCursor() {
    struct ImGuiIO* io = igGetIO();
    if (io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) return;

    ImGuiMouseCursor imgui_cursor = igGetMouseCursor();
    if (io->MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None) {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        HideCursor();
    } else {
        // Show OS mouse cursor
        ShowCursor();
    }

    switch (imgui_cursor) {
        case ImGuiMouseCursor_Arrow:
            SetMouseCursor(MOUSE_CURSOR_ARROW);
            break;
        case ImGuiMouseCursor_TextInput:
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            break;
        case ImGuiMouseCursor_Hand:
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            break;
        case ImGuiMouseCursor_NotAllowed:
            SetMouseCursor(MOUSE_CURSOR_NOT_ALLOWED);
            break;
        case ImGuiMouseCursor_ResizeAll:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
            break;
        case ImGuiMouseCursor_ResizeEW:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
            break;
        case ImGuiMouseCursor_ResizeNESW:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_NESW);
            break;
        case ImGuiMouseCursor_ResizeNS:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
            break;
        case ImGuiMouseCursor_ResizeNWSE:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
            break;
        default:
            break;
    }
}

#define MOUSE_EVENT(IO, IM_MOUSE, RL_MOUSE)              \
    if (IsMouseButtonPressed(RL_MOUSE))                  \
        ImGuiIO_AddMouseButtonEvent(IO, IM_MOUSE, true); \
    else if (IsMouseButtonReleased(RL_MOUSE))            \
        ImGuiIO_AddMouseButtonEvent(IO, IM_MOUSE, false);

static void ImGui_ImplRaylib_UpdateMousePosAndButtons() {
    struct ImGuiIO* io = igGetIO();

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by
    // user)
    if (io->WantSetMousePos)
        SetMousePosition((int)io->MousePos.x, (int)io->MousePos.y);
    else
        io->MousePos = (ImVec2){-FLT_MAX, -FLT_MAX};

    io->MouseDown[0] = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    io->MouseDown[1] = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    io->MouseDown[2] = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

    MOUSE_EVENT(io, ImGuiMouseButton_Left, MOUSE_BUTTON_LEFT);
    MOUSE_EVENT(io, ImGuiMouseButton_Right, MOUSE_BUTTON_RIGHT);
    MOUSE_EVENT(io, ImGuiMouseButton_Middle, MOUSE_BUTTON_MIDDLE);

    if (!IsWindowMinimized()) {
        ImGuiIO_AddMousePosEvent(io, (float)GetMouseX(), (float)GetMouseY());
    }
}

void ImGui_ImplRaylib_NewFrame() {
    struct ImGuiIO* io = igGetIO();

    io->Fonts->TexID = &g_FontTex.id;

    if (IsWindowFullscreen()) {
        io->DisplaySize =
            (ImVec2){(float)GetMonitorWidth(GetCurrentMonitor()), (float)GetMonitorHeight(GetCurrentMonitor())};
    } else {
        io->DisplaySize = (ImVec2){(float)GetScreenWidth(), (float)GetScreenHeight()};
    }

    Vector2 res_scale = GetWindowScaleDPI();

#if !defined(APPLE)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI)) res_scale = (Vector2){1.f, 1.f};
#endif

    io->DisplayFramebufferScale = (ImVec2){res_scale.x, res_scale.y};

    double current_time = GetTime();
    io->DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    ImGui_ImplRaylib_UpdateMousePosAndButtons();
    ImGui_ImplRaylib_UpdateMouseCursor();

    if (GetMouseWheelMove() > 0)
        io->MouseWheel += 1;
    else if (GetMouseWheelMove() < 0)
        io->MouseWheel -= 1;
}

#pragma region Keys
#define FOR_ALL_KEYS(X)                          \
    X(ImGuiKey_Tab, KEY_TAB);                    \
    X(ImGuiKey_LeftArrow, KEY_LEFT);             \
    X(ImGuiKey_RightArrow, KEY_RIGHT);           \
    X(ImGuiKey_UpArrow, KEY_UP);                 \
    X(ImGuiKey_DownArrow, KEY_DOWN);             \
    X(ImGuiKey_PageUp, KEY_PAGE_UP);             \
    X(ImGuiKey_PageDown, KEY_PAGE_DOWN);         \
    X(ImGuiKey_Home, KEY_HOME);                  \
    X(ImGuiKey_End, KEY_END);                    \
    X(ImGuiKey_Insert, KEY_INSERT);              \
    X(ImGuiKey_Delete, KEY_DELETE);              \
    X(ImGuiKey_Backspace, KEY_BACKSPACE);        \
    X(ImGuiKey_Space, KEY_SPACE);                \
    X(ImGuiKey_Enter, KEY_ENTER);                \
    X(ImGuiKey_Escape, KEY_ESCAPE);              \
    X(ImGuiKey_LeftCtrl, KEY_LEFT_CONTROL);      \
    X(ImGuiKey_LeftShift, KEY_LEFT_SHIFT);       \
    X(ImGuiKey_LeftAlt, KEY_LEFT_ALT);           \
    X(ImGuiKey_LeftSuper, KEY_LEFT_SUPER);       \
    X(ImGuiKey_RightCtrl, KEY_RIGHT_CONTROL);    \
    X(ImGuiKey_RightShift, KEY_RIGHT_SHIFT);     \
    X(ImGuiKey_RightAlt, KEY_RIGHT_ALT);         \
    X(ImGuiKey_RightSuper, KEY_RIGHT_SUPER);     \
    X(ImGuiKey_Menu, KEY_MENU);                  \
    X(ImGuiKey_0, KEY_ZERO);                     \
    X(ImGuiKey_1, KEY_ONE);                      \
    X(ImGuiKey_2, KEY_TWO);                      \
    X(ImGuiKey_3, KEY_THREE);                    \
    X(ImGuiKey_4, KEY_FOUR);                     \
    X(ImGuiKey_5, KEY_FIVE);                     \
    X(ImGuiKey_6, KEY_SIX);                      \
    X(ImGuiKey_7, KEY_SEVEN);                    \
    X(ImGuiKey_8, KEY_EIGHT);                    \
    X(ImGuiKey_9, KEY_NINE);                     \
    X(ImGuiKey_A, KEY_A);                        \
    X(ImGuiKey_B, KEY_B);                        \
    X(ImGuiKey_C, KEY_C);                        \
    X(ImGuiKey_D, KEY_D);                        \
    X(ImGuiKey_E, KEY_E);                        \
    X(ImGuiKey_F, KEY_F);                        \
    X(ImGuiKey_G, KEY_G);                        \
    X(ImGuiKey_H, KEY_H);                        \
    X(ImGuiKey_I, KEY_I);                        \
    X(ImGuiKey_J, KEY_J);                        \
    X(ImGuiKey_K, KEY_K);                        \
    X(ImGuiKey_L, KEY_L);                        \
    X(ImGuiKey_M, KEY_M);                        \
    X(ImGuiKey_N, KEY_N);                        \
    X(ImGuiKey_O, KEY_O);                        \
    X(ImGuiKey_P, KEY_P);                        \
    X(ImGuiKey_Q, KEY_Q);                        \
    X(ImGuiKey_R, KEY_R);                        \
    X(ImGuiKey_S, KEY_S);                        \
    X(ImGuiKey_T, KEY_T);                        \
    X(ImGuiKey_U, KEY_U);                        \
    X(ImGuiKey_V, KEY_V);                        \
    X(ImGuiKey_W, KEY_W);                        \
    X(ImGuiKey_X, KEY_X);                        \
    X(ImGuiKey_Y, KEY_Y);                        \
    X(ImGuiKey_Z, KEY_Z);                        \
    X(ImGuiKey_F1, KEY_F1);                      \
    X(ImGuiKey_F2, KEY_F2);                      \
    X(ImGuiKey_F3, KEY_F3);                      \
    X(ImGuiKey_F4, KEY_F4);                      \
    X(ImGuiKey_F5, KEY_F5);                      \
    X(ImGuiKey_F6, KEY_F6);                      \
    X(ImGuiKey_F7, KEY_F7);                      \
    X(ImGuiKey_F8, KEY_F8);                      \
    X(ImGuiKey_F9, KEY_F9);                      \
    X(ImGuiKey_F10, KEY_F10);                    \
    X(ImGuiKey_F11, KEY_F11);                    \
    X(ImGuiKey_F12, KEY_F12);                    \
    X(ImGuiKey_F13, KEY_PAUSE);                  \
    X(ImGuiKey_F14, KEY_PAUSE);                  \
    X(ImGuiKey_F15, KEY_PAUSE);                  \
    X(ImGuiKey_F16, KEY_PAUSE);                  \
    X(ImGuiKey_F17, KEY_PAUSE);                  \
    X(ImGuiKey_F18, KEY_PAUSE);                  \
    X(ImGuiKey_F19, KEY_PAUSE);                  \
    X(ImGuiKey_F20, KEY_PAUSE);                  \
    X(ImGuiKey_F21, KEY_PAUSE);                  \
    X(ImGuiKey_F22, KEY_PAUSE);                  \
    X(ImGuiKey_F23, KEY_PAUSE);                  \
    X(ImGuiKey_F24, KEY_PAUSE);                  \
    X(ImGuiKey_Apostrophe, KEY_APOSTROPHE);      \
    X(ImGuiKey_Comma, KEY_COMMA);                \
    X(ImGuiKey_Minus, KEY_MINUS);                \
    X(ImGuiKey_Period, KEY_PERIOD);              \
    X(ImGuiKey_Slash, KEY_SLASH);                \
    X(ImGuiKey_Semicolon, KEY_SEMICOLON);        \
    X(ImGuiKey_Equal, KEY_EQUAL);                \
    X(ImGuiKey_LeftBracket, KEY_LEFT_BRACKET);   \
    X(ImGuiKey_Backslash, KEY_BACKSLASH);        \
    X(ImGuiKey_RightBracket, KEY_RIGHT_BRACKET); \
    X(ImGuiKey_GraveAccent, KEY_GRAVE);          \
    X(ImGuiKey_CapsLock, KEY_CAPS_LOCK);         \
    X(ImGuiKey_ScrollLock, KEY_SCROLL_LOCK);     \
    X(ImGuiKey_NumLock, KEY_NUM_LOCK);           \
    X(ImGuiKey_PrintScreen, KEY_PRINT_SCREEN);   \
    X(ImGuiKey_Pause, KEY_PAUSE);                \
    X(ImGuiKey_Keypad0, KEY_KP_0);               \
    X(ImGuiKey_Keypad1, KEY_KP_1);               \
    X(ImGuiKey_Keypad2, KEY_KP_2);               \
    X(ImGuiKey_Keypad3, KEY_KP_3);               \
    X(ImGuiKey_Keypad4, KEY_KP_4);               \
    X(ImGuiKey_Keypad5, KEY_KP_5);               \
    X(ImGuiKey_Keypad6, KEY_KP_6);               \
    X(ImGuiKey_Keypad7, KEY_KP_7);               \
    X(ImGuiKey_Keypad8, KEY_KP_8);               \
    X(ImGuiKey_Keypad9, KEY_KP_9);               \
    X(ImGuiKey_KeypadDecimal, KEY_KP_DECIMAL);   \
    X(ImGuiKey_KeypadDivide, KEY_KP_DIVIDE);     \
    X(ImGuiKey_KeypadMultiply, KEY_KP_MULTIPLY); \
    X(ImGuiKey_KeypadSubtract, KEY_KP_SUBTRACT); \
    X(ImGuiKey_KeypadAdd, KEY_KP_ADD);           \
    X(ImGuiKey_KeypadEnter, KEY_KP_ENTER);       \
    X(ImGuiKey_KeypadEqual, KEY_KP_EQUAL);
#pragma endregion

#define SET_KEY_DOWN(IM_KEY, RL_KEY)           \
    if (IsKeyPressed(RL_KEY))                  \
        ImGuiIO_AddKeyEvent(io, IM_KEY, true); \
    else if (IsKeyReleased(RL_KEY))            \
        ImGuiIO_AddKeyEvent(io, IM_KEY, false);

bool ImGui_ImplRaylib_ProcessEvent() {
    struct ImGuiIO* io = igGetIO();

    FOR_ALL_KEYS(SET_KEY_DOWN);

    int keyPressed = GetCharPressed();
    while (keyPressed > 0) {
        // if (keyPressed >= KEY_A && keyPressed <= KEY_Z) {
        //     keyPressed += 26;
        // }
        ImGuiIO_AddInputCharacter(io, keyPressed);

        keyPressed = GetCharPressed();
    }

    bool shiftDown = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
    if (shiftDown != wasShiftDown) {
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Shift, shiftDown);
    }
    io->KeyShift = shiftDown;
    wasShiftDown = shiftDown;

    bool altDown = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
    if (altDown != wasAltDown) {
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Alt, altDown);
    }
    io->KeyShift = altDown;
    wasAltDown = altDown;

    bool ctrlDown = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
    if (ctrlDown != wasCtrlDown) {
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Ctrl, ctrlDown);
    }
    io->KeyShift = ctrlDown;
    wasCtrlDown = ctrlDown;

    bool superDown = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);
    if (superDown != wasSuperDown) {
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Super, superDown);
    }
    io->KeyShift = superDown;
    wasSuperDown = superDown;

    // bool altDown = IsKeyPressed(KEY_RIGHT_ALT) || IsKeyPressed(KEY_LEFT_ALT);
    // if (shiftDown != wasShiftDown)
    //     ImGuiIO_AddKeyEvent(io, ImGuiMod_Shift, shiftDown);
    // wasShiftDown = shiftDown;

    return true;
}

void draw_triangle_vertex(ImDrawVert idx_vert) {
    uint8_t r = idx_vert.col;
    uint8_t g = idx_vert.col >> 8;
    uint8_t b = idx_vert.col >> 16;
    uint8_t a = idx_vert.col >> 24;
    rlColor4ub(r, g, b, a);
    rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
    rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
}

void raylib_render_draw_triangles(unsigned int count, const ImDrawIdx* idx_buffer, const ImDrawVert* idx_vert,
                                  unsigned int texture_id) {
    // Draw the imgui triangle data
    for (unsigned int i = 0; i <= (count - 3); i += 3) {
        rlPushMatrix();
        rlBegin(RL_TRIANGLES);
        rlEnableTexture(texture_id);
        rlSetTexture(texture_id);

        ImDrawIdx index;
        ImDrawVert vertex;

        index = idx_buffer[i];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);

        index = idx_buffer[i + 2];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);

        index = idx_buffer[i + 1];
        vertex = idx_vert[index];
        draw_triangle_vertex(vertex);
        rlDisableTexture();
        rlEnd();
        rlPopMatrix();
    }
}

static void EnableScissor(float x, float y, float width, float height)
{
    rlEnableScissorTest();
    ImGuiIO* io = igGetIO();

    ImVec2 scale = io->DisplayFramebufferScale;
#if !defined(APPLE)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI))
    {
        scale.x = 1;
        scale.y = 1;
    }
#endif

    rlScissor((int)(x * scale.x),
        (int)((io->DisplaySize.y - (int)(y + height)) * scale.y),
        (int)(width * scale.x),
        (int)(height * scale.y));
}

void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data) {
    if (draw_data == NULL) {
        TraceLog(LOG_ERROR, "RenderDrawData(): draw_data was null!");
        return;
    }

    rlDisableBackfaceCulling();
    rlDrawRenderBatchActive();

    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists.Data[n];
        const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;  // vertex buffer generated by Dear ImGui
        const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;   // index buffer generated by Dear ImGui
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &(cmd_list->CmdBuffer.Data[cmd_i]);  // cmd_list->CmdBuffer->data[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                ImVec2 pos = draw_data->DisplayPos;
                float rectX = pcmd->ClipRect.x - pos.x;
                float rectY = pcmd->ClipRect.y - pos.y;
                float rectW = pcmd->ClipRect.z - rectX;
                float rectH = pcmd->ClipRect.w - rectY;
                EnableScissor(rectX, rectY, rectW, rectH);
                {
                    if (pcmd->TextureId != NULL) {
                        unsigned int id = *(unsigned int*)pcmd->TextureId;
                        raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer, id);
                    } else {
                        raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer, g_FontTex.id);
                    }
                }

            }
            idx_buffer += pcmd->ElemCount;

            rlDrawRenderBatchActive();
        }
    }

    rlSetTexture(0);
	rlDisableScissorTest();
	rlEnableBackfaceCulling();
}

void ImGui_ImplRaylib_Render() {
    igRender();
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
}