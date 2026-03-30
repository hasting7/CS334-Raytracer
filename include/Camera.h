#pragma once

#include "Vec3.h"

class Camera {
public:
    Camera() : Camera(Vec3(), Vec3(1,0,0), 60) {};
    Camera(Vec3 position, Vec3 direction, float fov);

    Vec3 position;
    Vec3 dir;
    float fov;
    float distance_to_screen;
private:

};