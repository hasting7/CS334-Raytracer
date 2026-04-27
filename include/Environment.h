#pragma once

#include <vector>
#include <memory>
#include <cstdint>

#include "Camera.h"
#include "Object.h"

class Environment {
public:
    Environment(int width, int height, int threads);
    
    void add_object(std::shared_ptr<Object> object);
    void render();

    std::vector<uint32_t> framebuffer;

    
private:
    void render_thread(int pixel_offset);
    // Returns a Vec3 representing floating-point RGB (0.0 to 1.0)
    // Vec3 compute_ray_color(const Ray& ray, int depth);
    Color trace(Ray &ray, int max_depth, double init_refractive_index);
    std::shared_ptr<Object> sample_light_source();
    HitRecord calculate_ray_collision(const Ray &ray);

    Camera camera;
    int width;
    int height;
    int thread_count;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Object>> light_sources;
    int light_source_inc = 0;
    
    // Quality settings
    int max_depth = 100;           // How many times a ray can bounce (Reflections)
    int samples_per_pixel = 250;  // Anti-Aliasing (Multisampling)
};