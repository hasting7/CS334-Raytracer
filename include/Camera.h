#pragma once

#include "Vec3.h"

class Camera {
public:
    Camera() :  : Camera(Vec3(), 60) {};
    Camera(Vec3 position, float fov);
private:
    Vec3 position;
    float fov;
};