#include "window.h"

// Contains platform-specific windowing functions
// Most of these map directly to Raylib and probably always will

void Window_CreateWindowed(gg_window_t* window, const char* title, uint32_t width, uint32_t height) {
    if (window->initialized) {
        CloseWindow();
    }

    InitWindow(width, height, title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    window->width = width;
    window->height = height;
    window->initialized = IsWindowReady();
    Window_SetFullscreen(window, false);

    // Disable ESC to exit
    SetExitKey(KEY_NULL);

    // Run as fast as we can!
    // TODO: Add configuration for this
    SetTargetFPS(INT32_MAX);
}

void Window_CreateFullscreen(gg_window_t* window, const char* title) {
    if (window->initialized) {
        CloseWindow();
    }

    uint32_t width = GetScreenWidth();
    uint32_t height = GetScreenHeight();
    InitWindow(width, height, title);
    Window_SetFullscreen(window, true);

    SetTargetFPS(60);
}

void Window_SetFullscreen(gg_window_t* window, bool fullscreen) {
    if (IsWindowFullscreen() != fullscreen) {
        ToggleFullscreen();
    }

    window->fullscreen = fullscreen;
}

void Window_Destroy(gg_window_t* window) { 
    CloseWindow();
}

uint32_t Window_GetWidth(gg_window_t* window) { return GetScreenWidth(); }

uint32_t Window_GetHeight(gg_window_t* window) { return GetScreenHeight(); }

float Window_GetDeltaTime(gg_window_t* window) { return GetFrameTime(); }

bool Window_ShouldClose(gg_window_t* window) { return WindowShouldClose(); }

bool Window_WasJustResized(gg_window_t* window) { return IsWindowResized(); }

void Window_BeginDrawing(gg_window_t* window) { BeginDrawing(); }

void Window_BeginCameraDrawing(gg_window_t* window, gg_camera_t* camera) {
    BeginMode2D((Camera2D){
        .offset = (Vector2){camera->offset_x, camera->offset_y},
        .rotation = camera->rotation,
        .target = (Vector2){camera->x, camera->y},
        .zoom = camera->zoom,
    });
}

void Window_BeginParallaxCameraDrawing(gg_window_t* window, gg_camera_t* camera, float parallax) {
    BeginMode2D((Camera2D){
        .offset = (Vector2){camera->offset_x, camera->offset_y},
        .rotation = camera->rotation,
        .target = (Vector2){camera->x * parallax, camera->y * parallax},
        .zoom = camera->zoom,
    });
}

void Window_EndDrawing(gg_window_t* window) { EndDrawing(); }

void Window_EndCameraDrawing(gg_window_t* window, gg_camera_t* camera) { EndMode2D(); }

void Window_ClearScreen(gg_window_t* window, gg_color_t color) {
    ClearBackground((Color){.r = color.r, .g = color.g, .b = color.b, .a = color.a});
}

void Window_DrawTexture(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y) {
    DrawTexture(texture->_handle, x, y, WHITE);
}

void Window_DrawTextureCentered(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y) {
    DrawTexture(texture->_handle, x - Texture_GetWidth(texture) / 2, y - (int32_t)Texture_GetHeight(texture) / 2, WHITE);
}

void Window_DrawTextureCenteredSR(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y, float scale,
                                  float rotation) {
    DrawTextureEx(
        texture->_handle,
        (Vector2){x - (Texture_GetWidth(texture) * scale) / 2.f, y - (Texture_GetHeight(texture) * scale) / 2.f},
        rotation, scale, WHITE);
}

void Window_DrawSubTexture(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y, uint32_t sx, uint32_t sy,
                           uint32_t sw, uint32_t sh) {
    DrawTextureRec(texture->_handle, (Rectangle){(float)sx, (float)sy, (float)sw, (float)sh}, (Vector2){(float)x, (float)y}, WHITE);
}

void Window_DrawRectangle(const gg_window_t* window, int32_t x, int32_t y, uint32_t w, uint32_t h, gg_color_t color) {
    DrawRectangle(x, y, w, h, (Color){color.r, color.g, color.b, color.a});
}

void Window_DrawDebugFPS(gg_window_t* window) { DrawFPS(10, 10); }

void Window_DrawSpaceOrigin(gg_window_t* window) {
    DrawLine(0, 0, 10, 0, RED);
    DrawLine(0, 0, 0, 10, GREEN);
}