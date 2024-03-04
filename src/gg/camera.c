#include "camera.h"

void Camera_Create(gg_camera_t* camera, uint32_t window_w, uint32_t window_h, float x, float y) {
    camera->x = x;
    camera->y = y;
    camera->rotation = 0.f;
    camera->offset_x = window_w / 2.f;
    camera->offset_y = window_h / 2.f;
    camera->zoom = 1.f;
}

void Camera_Destroy(gg_camera_t* camera) {
    // I'm a fraud
}