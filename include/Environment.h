#pragma once

#include "Camera.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"
#include <vector>

class Environment {
public:
    Environment(int width, int height);
    void render(std::vector<uint32_t> &framebuffer);
    Color shoot_ray(const Ray& ray, int depth);

    void add_object(std::unique_ptr<Object> object);
private:
	std::vector<std::unique_ptr<Object>> objects;
	Camera camera;
	int width;
	int height;
	int ray_depth;
	int rays_per_pixel;
};