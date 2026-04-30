#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"

class Camera {
public:
    Camera();
    Camera(Vec3 position, float aspect_ratio, float focal_distance, float aperture);

    void update_position(const Vec3 position);
    Vec3 get_position() const;
    void focus_on_sphere(const Sphere obj);
    Ray get_ray(float u, float v) const;
    Vec3 get_dir(float u, float v) const;
    
    float focal_distance;
    float aperture;

private:
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 origin;
};