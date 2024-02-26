#include "cimgui_impl_raylib.h"

#include <math.h>

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

    g_KeyMap[ImGuiKey_Tab] = KEY_TAB;
    g_KeyMap[ImGuiKey_LeftArrow] = KEY_LEFT;
    g_KeyMap[ImGuiKey_RightArrow] = KEY_RIGHT;
    g_KeyMap[ImGuiKey_UpArrow] = KEY_UP;
    g_KeyMap[ImGuiKey_DownArrow] = KEY_DOWN;
    g_KeyMap[ImGuiKey_PageUp] = KEY_PAGE_DOWN;
    g_KeyMap[ImGuiKey_PageDown] = KEY_PAGE_UP;
    g_KeyMap[ImGuiKey_Home] = KEY_HOME;
    g_KeyMap[ImGuiKey_End] = KEY_END;
    g_KeyMap[ImGuiKey_Insert] = KEY_INSERT;
    g_KeyMap[ImGuiKey_Delete] = KEY_DELETE;
    g_KeyMap[ImGuiKey_Backspace] = KEY_BACKSPACE;
    g_KeyMap[ImGuiKey_Space] = KEY_SPACE;
    g_KeyMap[ImGuiKey_Enter] = KEY_ENTER;
    g_KeyMap[ImGuiKey_Escape] = KEY_ESCAPE;
    g_KeyMap[ImGuiKey_KeypadEnter] = KEY_KP_ENTER;
    g_KeyMap[ImGuiKey_A] = KEY_A;
    g_KeyMap[ImGuiKey_C] = KEY_C;
    g_KeyMap[ImGuiKey_V] = KEY_V;
    g_KeyMap[ImGuiKey_X] = KEY_X;
    g_KeyMap[ImGuiKey_Y] = KEY_Y;
    g_KeyMap[ImGuiKey_Z] = KEY_Z;

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
}

static void ImGui_ImplRaylib_UpdateMousePosAndButtons() {
    struct ImGuiIO* io = igGetIO();

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by
    // user)
    if (io->WantSetMousePos)
        SetMousePosition((int)io->MousePos.x, (int)io->MousePos.y);
    else
        io->MousePos = (ImVec2){-FLT_MAX, -FLT_MAX};

    io->MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io->MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io->MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

    if (!IsWindowMinimized()) {
        io->MousePos = (ImVec2){(float)GetTouchX(), (float)GetTouchY()};
    }
}

void ImGui_ImplRaylib_NewFrame() {
    struct ImGuiIO* io = igGetIO();

    io->Fonts->TexID = &g_FontTex.id;

    io->DisplaySize = (ImVec2){(float)GetScreenWidth(), (float)GetScreenHeight()};

    double current_time = GetTime();
    io->DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    io->KeyCtrl = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
    io->KeyShift = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
    io->KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
    io->KeySuper = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

    ImGui_ImplRaylib_UpdateMousePosAndButtons();
    ImGui_ImplRaylib_UpdateMouseCursor();

    if (GetMouseWheelMove() > 0)
        io->MouseWheel += 1;
    else if (GetMouseWheelMove() < 0)
        io->MouseWheel -= 1;
}

#pragma region Keys
#define FOR_ALL_KEYS(X)                       \
    X(ImGuiKey_Tab, KEY_TAB);                 \
    X(ImGuiKey_LeftArrow, KEY_LEFT);          \
    X(ImGuiKey_RightArrow, KEY_RIGHT);        \
    X(ImGuiKey_LeftArrow, KEY_UP);            \
    X(ImGuiKey_LeftArrow, KEY_DOWN);          \
    X(ImGuiKey_LeftArrow, KEY_PAGE_UP);       \
    X(ImGuiKey_LeftArrow, KEY_PAGE_DOWN);     \
    X(ImGuiKey_LeftArrow, KEY_HOME);          \
    X(ImGuiKey_LeftArrow, KEY_END);           \
    X(ImGuiKey_LeftArrow, KEY_INSERT);        \
    X(ImGuiKey_LeftArrow, KEY_DELETE);        \
    X(ImGuiKey_LeftArrow, KEY_BACKSPACE);     \
    X(ImGuiKey_LeftArrow, KEY_SPACE);         \
    X(ImGuiKey_LeftArrow, KEY_ENTER);         \
    X(ImGuiKey_LeftArrow, KEY_ESCAPE);        \
    X(ImGuiKey_LeftArrow, KEY_LEFT_CONTROL);  \
    X(ImGuiKey_LeftArrow, KEY_LEFT_SHIFT);    \
    X(ImGuiKey_LeftArrow, KEY_LEFT_ALT);      \
    X(ImGuiKey_LeftArrow, KEY_LEFT_SUPER);    \
    X(ImGuiKey_LeftArrow, KEY_RIGHT_CONTROL); \
    X(ImGuiKey_LeftArrow, KEY_RIGHT_SHIFT);   \
    X(ImGuiKey_LeftArrow, KEY_RIGHT_ALT);     \
    X(ImGuiKey_LeftArrow, KEY_RIGHT_SUPER);   \
    X(ImGuiKey_LeftArrow, KEY_MENU);          \
    X(ImGuiKey_LeftArrow, KEY_ZERO);          \
    X(ImGuiKey_LeftArrow, KEY_ONE);           \
    X(ImGuiKey_LeftArrow, KEY_TWO);           \
    X(ImGuiKey_LeftArrow, KEY_THREE);         \
    X(ImGuiKey_LeftArrow, KEY_FOUR);          \
    X(ImGuiKey_LeftArrow, KEY_FIVE);          \
    X(ImGuiKey_LeftArrow, KEY_SIX);           \
    X(ImGuiKey_LeftArrow, KEY_SEVEN);         \
    X(ImGuiKey_LeftArrow, KEY_EIGHT);         \
    X(ImGuiKey_LeftArrow, KEY_NINE);          \
    X(ImGuiKey_LeftArrow, KEY_A);             \
    X(ImGuiKey_LeftArrow, KEY_B);             \
    X(ImGuiKey_LeftArrow, KEY_C);             \
    X(ImGuiKey_LeftArrow, KEY_D);             \
    X(ImGuiKey_LeftArrow, KEY_E);             \
    X(ImGuiKey_LeftArrow, KEY_F);             \
    X(ImGuiKey_LeftArrow, KEY_G);             \
    X(ImGuiKey_LeftArrow, KEY_H);             \
    X(ImGuiKey_LeftArrow, KEY_I);             \
    X(ImGuiKey_LeftArrow, KEY_J);             \
    X(ImGuiKey_LeftArrow, KEY_K);             \
    X(ImGuiKey_LeftArrow, KEY_L);             \
    X(ImGuiKey_LeftArrow, KEY_M);             \
    X(ImGuiKey_LeftArrow, KEY_N);             \
    X(ImGuiKey_LeftArrow, KEY_O);             \
    X(ImGuiKey_LeftArrow, KEY_P);             \
    X(ImGuiKey_LeftArrow, KEY_Q);             \
    X(ImGuiKey_LeftArrow, KEY_R);             \
    X(ImGuiKey_LeftArrow, KEY_S);             \
    X(ImGuiKey_LeftArrow, KEY_T);             \
    X(ImGuiKey_LeftArrow, KEY_U);             \
    X(ImGuiKey_LeftArrow, KEY_V);             \
    X(ImGuiKey_LeftArrow, KEY_W);             \
    X(ImGuiKey_LeftArrow, KEY_X);             \
    X(ImGuiKey_LeftArrow, KEY_Y);             \
    X(ImGuiKey_LeftArrow, KEY_Z);             \
    X(ImGuiKey_LeftArrow, KEY_F1);            \
    X(ImGuiKey_LeftArrow, KEY_F2);            \
    X(ImGuiKey_LeftArrow, KEY_F3);            \
    X(ImGuiKey_LeftArrow, KEY_F4);            \
    X(ImGuiKey_LeftArrow, KEY_F5);            \
    X(ImGuiKey_LeftArrow, KEY_F6);            \
    X(ImGuiKey_LeftArrow, KEY_F7);            \
    X(ImGuiKey_LeftArrow, KEY_F8);            \
    X(ImGuiKey_LeftArrow, KEY_F9);            \
    X(ImGuiKey_LeftArrow, KEY_F10);           \
    X(ImGuiKey_LeftArrow, KEY_F11);           \
    X(ImGuiKey_LeftArrow, KEY_F12);           \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_APOSTROPHE);    \
    X(ImGuiKey_LeftArrow, KEY_COMMA);         \
    X(ImGuiKey_LeftArrow, KEY_MINUS);         \
    X(ImGuiKey_LeftArrow, KEY_PERIOD);        \
    X(ImGuiKey_LeftArrow, KEY_SLASH);         \
    X(ImGuiKey_LeftArrow, KEY_SEMICOLON);     \
    X(ImGuiKey_LeftArrow, KEY_EQUAL);         \
    X(ImGuiKey_LeftArrow, KEY_LEFT_BRACKET);  \
    X(ImGuiKey_LeftArrow, KEY_BACKSLASH);     \
    X(ImGuiKey_LeftArrow, KEY_RIGHT_BRACKET); \
    X(ImGuiKey_LeftArrow, KEY_GRAVE);         \
    X(ImGuiKey_LeftArrow, KEY_CAPS_LOCK);     \
    X(ImGuiKey_LeftArrow, KEY_SCROLL_LOCK);   \
    X(ImGuiKey_LeftArrow, KEY_NUM_LOCK);      \
    X(ImGuiKey_LeftArrow, KEY_PRINT_SCREEN);  \
    X(ImGuiKey_LeftArrow, KEY_PAUSE);         \
    X(ImGuiKey_LeftArrow, KEY_KP_0);          \
    X(ImGuiKey_LeftArrow, KEY_KP_1);          \
    X(ImGuiKey_LeftArrow, KEY_KP_2);          \
    X(ImGuiKey_LeftArrow, KEY_KP_3);          \
    X(ImGuiKey_LeftArrow, KEY_KP_4);          \
    X(ImGuiKey_LeftArrow, KEY_KP_5);          \
    X(ImGuiKey_LeftArrow, KEY_KP_6);          \
    X(ImGuiKey_LeftArrow, KEY_KP_7);          \
    X(ImGuiKey_LeftArrow, KEY_KP_8);          \
    X(ImGuiKey_LeftArrow, KEY_KP_9);          \
    X(ImGuiKey_LeftArrow, KEY_KP_DECIMAL);    \
    X(ImGuiKey_LeftArrow, KEY_KP_DIVIDE);     \
    X(ImGuiKey_LeftArrow, KEY_KP_MULTIPLY);   \
    X(ImGuiKey_LeftArrow, KEY_KP_SUBTRACT);   \
    X(ImGuiKey_LeftArrow, KEY_KP_ADD);        \
    X(ImGuiKey_LeftArrow, KEY_KP_ENTER);      \
    X(ImGuiKey_LeftArrow, KEY_KP_EQUAL);
#pragma endregion

#define SET_KEY_DOWN(IM_KEY, RL_KEY) io->KeysData[IM_KEY - ImGuiKey_NamedKey_BEGIN].Down = IsKeyDown(RL_KEY)

bool ImGui_ImplRaylib_ProcessEvent() {
    struct ImGuiIO* io = igGetIO();

    FOR_ALL_KEYS(SET_KEY_DOWN);

    int keyPressed = GetKeyPressed();
    if (keyPressed > 0) {
        ImGuiIO_AddInputCharacter(io, keyPressed);
    }

    return true;
}

void draw_triangle_vertex(ImDrawVert idx_vert) {
    uint8_t r = idx_vert.col;
    uint8_t g = idx_vert.col >> 8;
    uint8_t b = idx_vert.col >> 16;
    uint8_t a = idx_vert.col >> 24;
    rlColor4ub(r, g, b, a);
    rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
    rlVertex2f(roundf(idx_vert.pos.x), roundf(idx_vert.pos.y));
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

void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data) {
    if (draw_data == NULL) {
        TraceLog(LOG_ERROR, "RenderDrawData(): draw_data was null!");
        return;
    }

    // rlDisableBackfaceCulling();
    rlSetBlendMode(RL_BLEND_ALPHA);
    rlEnableColorBlend();
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
                int rectX = (int)(pcmd->ClipRect.x - pos.x);
                int rectY = (int)(pcmd->ClipRect.y - pos.y);
                int rectW = (int)(pcmd->ClipRect.z - rectX);
                int rectH = (int)(pcmd->ClipRect.w - rectY);
                BeginScissorMode(rectX, rectY, rectW, rectH);
                {
                    if (pcmd->TextureId != NULL) {
                        unsigned int id = *(unsigned int*)pcmd->TextureId;
                        raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer, g_FontTex.id);
                    } else {
                        raylib_render_draw_triangles(pcmd->ElemCount, idx_buffer, vtx_buffer, g_FontTex.id);
                    }
                }
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    EndScissorMode();
    // rlEnableBackfaceCulling();
}

void ImGui_ImplRaylib_Render() {
    igRender();
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
}