#pragma once

#include "Camera.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"
#include "Vec3.h"
#include <vector>

class Environment {
public:
    Environment(int width, int height);
    void render(std::vector<uint32_t> &framebuffer);
    Color shoot_ray(const Ray& ray, int depth);

    void add_object(std::unique_ptr<Object> object);
    void add_point_light(const Vec3& position);
private:
	// all objects in the scene that can be seen / have material (NOT lights)
	std::vector<std::unique_ptr<Object>> objects;
	
	// all point lights in the scene, later we can have light as a material property
	std::vector<Vec3> point_lights;
	Camera camera;
	int width;
	int height;
	int ray_depth;
	int rays_per_pixel;
};