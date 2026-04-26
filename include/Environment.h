#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Camera.h"
#include "Object.h"
#include "Light.h"

class Environment {
public:
    Environment(int width, int height);
    
    void add_object(std::shared_ptr<Object> object);
    void add_light(const Light& light);
    void render(std::vector<uint32_t> &framebuffer);
    
private:
    // Returns a Vec3 representing floating-point RGB (0.0 to 1.0)
    Vec3 compute_ray_color(const Ray& ray, int depth);

    Camera camera;
    int width;
    int height;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<Light> lights;
    
    // Quality settings
    int max_depth = 4;           // How many times a ray can bounce (Reflections)
    int samples_per_pixel = 300;  // Anti-Aliasing (Multisampling)
};