#include "camera.h"

void Camera_Create(gg_camera_t* camera, uint32_t window_w, uint32_t window_h, uint32_t x, uint32_t y) {
    camera->x = x;
    camera->y = y;
    camera->rotation = 0.f;
    camera->offset_x = window_w / 2.f;
    camera->offset_y = window_h / 2.f;
    camera->zoom = 1.f;
}