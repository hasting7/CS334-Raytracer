#define NOMINMAX // Fixes Windows macro conflict with std::min/max
#include "Environment.h"
#include "Color.h"

#include <cmath>
#include <algorithm>
#include <random>

// Helper to generate random numbers for Anti-Aliasing
inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

Environment::Environment(int width, int height) : width(width), height(height) {
    float aspect_ratio = (float)width / height;
    this->camera = Camera(Vec3(0, 0, 0), aspect_ratio);
}

void Environment::add_object(std::shared_ptr<Object> object) {
    objects.push_back(object);
}

void Environment::add_light(const Light& light) {
    lights.push_back(light);
}

Vec3 Environment::compute_ray_color(const Ray& ray, int depth) {
    // If we've bounced too many times, stop reflecting
    if (depth >= max_depth) {
        return Vec3(0, 0, 0); 
    }

    HitRecord rec;
    bool hit_anything = false;
    float closest_so_far = 1e9f;
    HitRecord temp_rec;

    for (const auto& object : objects) {
        if (object->hit(ray, 0.001f, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    if (hit_anything) {
        float total_diffuse = 0.0f;
        
        for (const auto& light : lights) {
            Vec3 light_dir = (light.position - rec.point).normalize();
            
            // Shadows
            Ray shadow_ray(rec.point + rec.normal * 0.001f, light_dir);
            bool in_shadow = false;
            HitRecord shadow_rec;
            float distance_to_light = (light.position - rec.point).magnitude();
            
            for (const auto& object : objects) {
                if (object->hit(shadow_ray, 0.001f, distance_to_light, shadow_rec)) {
                    in_shadow = true;
                    break;
                }
            }
            
            if (!in_shadow) {
                float diff = std::max(0.0f, rec.normal * light_dir);
                total_diffuse += diff * light.intensity;
            }
        }
        
        float lighting = rec.material.ambient + rec.material.diffuse * total_diffuse;
        lighting = std::min(1.0f, lighting);
        
        // Base color scaled from 0.0 to 1.0
        Vec3 base_color(
            (rec.material.color.r / 255.0f) * lighting,
            (rec.material.color.g / 255.0f) * lighting,
            (rec.material.color.b / 255.0f) * lighting
        );

        // Reflection Calculation
        if (rec.material.reflectivity > 0.0f) {
            Vec3 reflect_dir = Vec3::reflect(ray.direction.normalize(), rec.normal);
            Ray reflected_ray(rec.point + rec.normal * 0.001f, reflect_dir);
            
            // Recursively get the color from the bounce
            Vec3 reflected_color = compute_ray_color(reflected_ray, depth + 1);
            
            // Blend the object's color with the reflection
            base_color = base_color * (1.0f - rec.material.reflectivity) + reflected_color * rec.material.reflectivity;
        }

        return base_color;
    }

    // Background Gradient (Converted to float range 0.0 - 1.0)
    Vec3 unit_direction = ray.direction.normalize();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return Vec3(1.0f - t + t * 0.5f, 1.0f - t + t * 0.7f, 1.0f);
}

void Environment::render(std::vector<uint32_t> &framebuffer) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Vec3 pixel_color(0, 0, 0);

            // ANTI-ALIASING: Sample the same pixel multiple times with tiny random offsets
            for (int s = 0; s < samples_per_pixel; s++) {
                float u = (float(i) + random_float()) / (width - 1);
                float v = (float(height - 1 - j) + random_float()) / (height - 1);
                
                Ray ray = camera.get_ray(u, v);
                pixel_color += compute_ray_color(ray, 0);
            }

            // Average the samples
            pixel_color /= (float)samples_per_pixel;

            // GAMMA CORRECTION: (gamma = 2.0 -> apply square root)
            float r = std::sqrt(std::max(0.0f, std::min(1.0f, pixel_color.x)));
            float g = std::sqrt(std::max(0.0f, std::min(1.0f, pixel_color.y)));
            float b = std::sqrt(std::max(0.0f, std::min(1.0f, pixel_color.z)));

            Color final_color((int)(r * 255), (int)(g * 255), (int)(b * 255));
            framebuffer[j * width + i] = final_color.to_int();
        }
    }
}