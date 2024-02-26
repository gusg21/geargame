#if !defined(GG_CAMERA_H)
#define GG_CAMERA_H

#include <stdint.h>

typedef struct gg_camera {
    float x, y;
    float zoom;
    float offset_x, offset_y;
    float rotation;
} gg_camera_t;

void Camera_Create(gg_camera_t* camera, uint32_t window_w, uint32_t window_h, uint32_t x, uint32_t y);

#endif  // GG_CAMERA_H
