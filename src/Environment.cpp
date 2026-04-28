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
    this->camera = Camera(Vec3(0, 4, 12), aspect_ratio, 12.0f, 0.15f); // 0.08f
}

void Environment::add_object(std::shared_ptr<Object> object) {
    objects.push_back(object);
    if (object->material.emission_strength != 0.0f) {
        light_sources.push_back(object);
    }
}


Color Environment::trace(Ray &ray, int max_depth, double init_refractive_index) {
    Color incoming_light = Color(0.0f,0.0f,0.0f);
    Color ray_color = Color(1.0f,1.0f,1.0f);
    HitRecord record;
    double refractive_index = init_refractive_index;
    int ray_count = 0;
    bool needs_direct_light_sample = true;

    for (; ray_count < max_depth; ray_count++) {
        // terminate early if no more light can do anything to scene
        float throughput_strength = std::max(ray_color.r, std::max(ray_color.g, ray_color.b));

        if (throughput_strength < 0.001f) {
            break;
        }


        needs_direct_light_sample = true;
        // // 1. Do direct light sampling
        // std::shared_ptr<Object> obj = sample_light_source();
        // std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(obj);
        // // these are all sphers but maybe in the future will be more

        // Vec3 sphere_to_position = (ray.origin - sphere->center);
        // Vec3 position_on_light = random_in_hemisphere(sphere_to_position);

        // Ray direct_calc_ray = Ray(ray.origin, position_on_light - ray.origin);
        // record = calculate_ray_collision(direct_calc_ray);
        // if (record.hit) {

        // }

        // do random light sampling
        record = calculate_ray_collision(ray);
        if (record.hit) {
            Material material = record.material;

            if (material.transmission > 0.0f) {
                needs_direct_light_sample = false;
                // determine n1 and n2
                double n1 = refractive_index;
                double n2 = material.refractive_index;

                // we hit back face
                if (!record.front_face) {
                    n2 = init_refractive_index;

                }

                // calculate outgoing dir
                ray.origin = record.point;
                Vec3 refracted_dir = apply_snells(ray.direction, record.normal, n1, n2);

                // cloudyness
                if (material.glass_roughness > 0.0f) {
                    refracted_dir = (
                        refracted_dir +
                        random_in_hemisphere(refracted_dir) * material.glass_roughness
                    ).normalize();
                }

                ray.direction = refracted_dir;

                // update refractive index to the new medium
                refractive_index = n2;

                if (!record.front_face) {
                    ray_color *= material.tint;
                }
            } else {

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
                    ray_color *= material.specular_color; 
                } else {
                    ray_color *= material.color;
                }
            }

            if (needs_direct_light_sample) {
                // do it
            }


        } else {
            Color background_top(0.02f, 0.02f, 0.04f);
            Color background_bottom(0.10f, 0.10f, 0.12f);

            Vec3 unit_dir = ray.direction.normalize();
            double t = 0.5f * (unit_dir.y + 1.0f);
            incoming_light += background_bottom * (1.0f - t) + background_top * t;
            // no hit
            break;
        }

    }
    if ((ray_count == 0) && (!record.hit)) {
        Color background_top(0.02f, 0.02f, 0.04f);
        Color background_bottom(0.10f, 0.10f, 0.12f);

        Vec3 unit_dir = ray.direction.normalize();
        double t = 0.5f * (unit_dir.y + 1.0f);
        incoming_light += background_bottom * (1.0f - t) + background_top * t;
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

std::shared_ptr<Object> Environment::sample_light_source() {
    light_source_inc = (light_source_inc + 1) % light_sources.size();
    return light_sources[light_source_inc];
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

            for (int s = 0; s < samples_per_pixel; s++) {
                float u = (float(i) + random_float()) / (width - 1);
                float v = (float(height - 1 - j) + random_float()) / (height - 1);

                Vec3 dir = camera.get_dir(u, v).normalize();

                Vec3 focal_point = camera.origin + dir * camera.focal_distance;
                
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

                Color ray_color = trace(final_ray, max_depth, 1.0003);
                pixel_color += ray_color;
            }


            // Average the samples
            pixel_color /= (float)samples_per_pixel;

            framebuffer[j * width + i] = pixel_color.to_int();
        }
    }
}