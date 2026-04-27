#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Camera.h"
#include "Object.h"
#include "Light.h"

class Environment {
public:
    Environment(int width, int height, int threads);
    
    void add_object(std::shared_ptr<Object> object);
    void add_light(const Light& light);
    void render();

    std::vector<uint32_t> framebuffer;

    
private:
    void render_thread(int pixel_offset);
    // Returns a Vec3 representing floating-point RGB (0.0 to 1.0)
    // Vec3 compute_ray_color(const Ray& ray, int depth);
    Color trace(Ray &ray, int max_depth);
    HitRecord calculate_ray_collision(const Ray &ray);

    Camera camera;
    int width;
    int height;
    int thread_count;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<Light> lights;
    
    // Quality settings
    int max_depth = 50;           // How many times a ray can bounce (Reflections)
    int samples_per_pixel = 100;  // Anti-Aliasing (Multisampling)
};