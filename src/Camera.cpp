#include "Camera.h"

Camera::Camera() : Camera(Vec3(0, 0, 0), 800.0f / 600.0f) {}

Camera::Camera(Vec3 position, float aspect_ratio) {
    this->origin = position;
    
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;

    this->horizontal = Vec3(viewport_width, 0.0f, 0.0f);
    this->vertical = Vec3(0.0f, viewport_height, 0.0f);
    this->lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vec3(0.0f, 0.0f, 1.0f);

    focal_length = 1.0f;
    aperture = 1.0f;
}

Ray Camera::get_ray(float u, float v) const {
    Vec3 direction = lower_left_corner + horizontal * u + vertical * v - origin;
    return Ray(origin, direction.normalize());
}

Vec3 Camera::get_dir(float u, float v) const {
    return lower_left_corner + horizontal * u + vertical * v - origin;
}