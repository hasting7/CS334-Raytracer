#pragma once

#include "Vec3.h"
#include "Ray.h"

class Camera {
public:
    Camera();
    Camera(Vec3 position, float aspect_ratio, float focal_distance, float aperture);
    
    Ray get_ray(float u, float v) const;
    Vec3 get_dir(float u, float v) const;
    float focal_distance;
    float aperture;
    Vec3 origin;
private:
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
};