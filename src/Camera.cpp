#include "Camera.h"
#include "Vec3.h"
#include "Ray.h"

#include <cmath>

/*
x: pointing right
y: pointing up
z: away from scren
*/

Camera::Camera(Vec3 position, Vec3 direction, float fov, int screen_width, int screen_height)
    : position(position), dir(direction.normalize()), fov(fov),
      screen_width(screen_width), screen_height(screen_height)
{
    distance_to_screen = 1.0f;

    Vec3 world_up(0, 1, 0);

    Vec3 forward = dir;
    Vec3 right = Vec3::cross(forward, world_up).normalize();
    Vec3 up = Vec3::cross(right, forward).normalize();

    Vec3 viewport_center = position + forward * distance_to_screen;

    float fov_radians = fov * 3.14159265358979323846f / 180.0f;
    viewport_height = 2.0f * distance_to_screen * std::tan(fov_radians / 2.0f);
    viewport_width = (static_cast<float>(screen_width) / static_cast<float>(screen_height)) * viewport_height;

    dx = right * (viewport_width / static_cast<float>(screen_width));
    dy = -up * (viewport_height / static_cast<float>(screen_height));

    viewport_origin = viewport_center - right * (viewport_width / 2.0f) + up * (viewport_height / 2.0f);
}

Ray Camera::make_ray(int pix_x, int pix_y, float x_offset, float y_offset) const {
	Vec3 viewport_pos = viewport_origin + (dx * (static_cast<float>(pix_x) + x_offset)) + (dy * (static_cast<float>(pix_y) + y_offset));
	return Ray(position, (viewport_pos - position).normalize());
}