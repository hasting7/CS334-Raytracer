#pragma once

#include "Camera.h"
#include "Ray.h"
#include "Color.h"
#include "Object.h"
#include <vector>
#include <random>

class Environment {
public:
    Environment(int width, int height);
    void render(std::vector<uint32_t> &framebuffer);
    Color shoot_ray(const Ray& ray, int depth, int branch, bool from_camera);

    void add_object(std::unique_ptr<Object> object);
private:
	std::vector<std::unique_ptr<Object>> objects;
	Camera camera;
	int width;
	int height;
	int ray_depth;
	int rays_per_pixel;
	std::random_device rd; 
	std::mt19937_64 gen;
	std::uniform_real_distribution<float> uniform_dist; 
	std::vector<Color> color_history;
	float alpha;
	int frame_count;
};