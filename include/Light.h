#pragma once

#include "Vec3.h"

class Light {
public:
    Light(Vec3 position, float intensity) : position(position), intensity(intensity) {}
    Vec3 position;
    float intensity;
};