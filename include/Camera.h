#pragma once

#include "Vec3.h"
#include "Ray.h"


class Camera {
public:
    Camera() {};
    Camera(Vec3 position, Vec3 direction, float fov, int screen_width, int screen_height);

    Ray make_ray(int pix_x, int pix_y, float x_offset, float y_offset) const;

    Vec3 position;
    Vec3 dir;
    float fov;
    float distance_to_screen;
private:
    int screen_width;
    int screen_height;
    float viewport_width;
    float viewport_height;

    Vec3 dx;
    Vec3 dy;
    Vec3 viewport_origin;
};