#pragma once

#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Camera();
    Camera(Vec3 position, float aspect_ratio);
    
    Ray get_ray(float u, float v) const;
private:
    Vec3 origin;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

    float focal_length;
    float aperture;
};