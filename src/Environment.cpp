#define NOMINMAX // Fixes Windows macro conflict with std::min/max
#include "Environment.h"
#include "Color.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <thread>

// Helper to generate random numbers for Anti-Aliasing
inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

Environment::Environment(int width, int height, int thread_count) : width(width), height(height), framebuffer(width * height, 0xFF000000), thread_count(thread_count) {
    float aspect_ratio = (float)width / height;
    this->camera = Camera(Vec3(0, 4, 5), aspect_ratio, 5, 0.11f); // 0.08f
}

void Environment::add_object(std::shared_ptr<Object> object) {
    objects.push_back(object);
}

void Environment::add_light(const Light& light) {
    lights.push_back(light);
}


Color Environment::trace(Ray &ray, int max_depth) {
    Color incoming_light = Color(0.0f,0.0f,0.0f);
    Color ray_color = Color(1.0f,1.0f,1.0f);
    HitRecord record;
    int ray_count = 0;

    for (; ray_count < max_depth; ray_count++) {
        record = calculate_ray_collision(ray);
        if (record.hit) {
            Material material = record.material;
            Vec3 next_dir, inital_diffusion_vec;
            ray.origin = record.point;

            inital_diffusion_vec = (record.normal + random_vec3()).normalize();

            bool is_specular_reflection = random_float() < material.specular_probability;

            // check if object is reflective or not
            if (is_specular_reflection) {
                ray.direction = (1.0f - material.reflectivity) * inital_diffusion_vec + material.reflectivity * ray.direction.reflect(record.normal);
            } else {
                // not reflective
                ray.direction = inital_diffusion_vec;
            }
            // determine new incoming light
            Color emitted_light = material.emission_color * material.emission_strength;
            // consider light strength
            // add light, and tint it based on all the light we have seen so far
            incoming_light += emitted_light * ray_color;

            if (is_specular_reflection) {
                ray_color *= Color(1.0f, 1.0f, 1.0f); 
            } else {
                ray_color *= material.color;
            }


        } else {
            // no hit
            break;
        }

    }
    if ((ray_count == 0) && (!record.hit)) {
        // never hit anything, return sky color
        return Color(0,0,0);
        Vec3 unit_direction = ray.direction.normalize();
        float t = 0.5f * (unit_direction.y + 1.0f);

        Color white(1.0f, 1.0f, 1.0f);
        Color sky_blue(0.0f, 0.9f, 1.0f);

        return white * (1.0f - t) + sky_blue * t;
    }

    return incoming_light;
}

HitRecord Environment::calculate_ray_collision(const Ray &ray) {
    HitRecord record;
    for (const auto& object : objects) {
        object->hit(ray, 0.0001f, record);
    }
    return record;

}


void Environment::render() {
    std::vector<std::thread> threads;

    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back(&Environment::render_thread, this, i);
    }

    for (auto& t : threads) {
        t.join();
    }
}

void Environment::render_thread(int pixel_offset) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // skip for multithreading
            if ((j * width + i) % thread_count != pixel_offset) continue;

            Color pixel_color(0, 0, 0);

            // find convergence point
            Vec3 dir = camera.get_dir(float(i) / (width - 1), float(height - 1 - j) / (height - 1));
            Vec3 focal_point = camera.origin + dir * camera.focal_distance;

            for (int s = 0; s < samples_per_pixel; s++) {
                // float u = (float(i) + random_float()) / (width - 1);
                // float v = (float(height - 1 - j) + random_float()) / (height - 1);
                
                // Ray ray = camera.get_ray(u, v);

                // 1. calculate random offset using aperture 
                // 2. ray origin -> add to camera->origin
                // 3. ray dir -> convergence_point

                // calculate random r and theta
                // r -> [0, apeture]
                // theta -> [0, 2pi] (radians) 

                float theta = random_float() * 3.14159265358979323846f * 2.0f;
                float r = (1.0f - random_float()) * camera.aperture;

                float dx = std::cos(theta) * r;
                float dy = std::sin(theta) * r;

                // in the future maybe make dx and dy relative to camera coords rather than just x, y directly
                Vec3 new_origin = camera.origin + Vec3(dx, dy, 0);

                Ray final_ray = Ray(new_origin, focal_point - new_origin);

                Color ray_color = trace(final_ray, max_depth);
                pixel_color.r += ray_color.r;
                pixel_color.g += ray_color.g;
                pixel_color.b += ray_color.b;

            }


            // Average the samples
            pixel_color /= (float)samples_per_pixel;

            framebuffer[j * width + i] = pixel_color.to_int();
        }
    }
}