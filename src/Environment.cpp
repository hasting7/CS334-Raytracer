#define NOMINMAX // Fixes Windows macro conflict with std::min/max
#include "Environment.h"
#include "Color.h"
#include "Vec3.h"
#include "Ray.h"

#include <random>

#include <cmath>
#include <algorithm>
#include <random>
#include <thread>


Environment::Environment(int width, int height, int thread_count, int max_depth, int samples_per_pixel) : 
    width(width), height(height), framebuffer(width * height, 0xFF000000), thread_count(thread_count),
    max_depth(max_depth), samples_per_pixel(samples_per_pixel)
{
    float aspect_ratio = (float)width / height;
    this->camera = Camera(Vec3(0, 0, 0), aspect_ratio, 0.0f, 0.0f);
}

void Environment::add_object(std::shared_ptr<Object> object) {
    std::shared_ptr<Plane> plane = std::dynamic_pointer_cast<Plane>(object);
    if (plane && plane->material.emission_strength > 0.0f) {
        plane->material.emission_strength = 0.0f;
        printf("PLANE OBJ cannot emit light, setting emission_strength to 0.\n");
    }

    objects.push_back(object);
    if (object->material.emission_strength != 0.0f) {
        light_sources.push_back(object);
    }
}


Color Environment::trace(Ray& ray, int max_depth, double init_refractive_index) {
    Color incoming_light(0.0f, 0.0f, 0.0f);
    Color ray_color(1.0f, 1.0f, 1.0f);
    Vec3 incoming_dir = ray.direction;

    double refractive_index = init_refractive_index;

    for (int ray_count = 0; ray_count < max_depth; ray_count++) {
        incoming_dir = ray.direction;
        HitRecord record = calculate_ray_collision(ray);

        if (!record.hit) {
            // maybe add natural lighting later
            break;
        }

        Material material = record.material;

        // If we hit a light, collect its emission and stop.
        if (material.emission_strength > 0.0f) {
            Color emitted_light =
                material.emission_color * material.emission_strength;

            incoming_light += emitted_light * ray_color;
            break;
        }

        // Glass branch
        if (material.transmission > 0.0f) {
            double n1 = refractive_index;

            double n2 = record.front_face
                ? material.refractive_index
                : init_refractive_index;

            Vec3 refracted_dir = apply_snells(
                ray.direction.normalize(), record.normal, n1, n2
            ).normalize();

            if (material.glass_roughness > 0.0f) {
                refracted_dir = (
                    refracted_dir + random_in_hemisphere(refracted_dir) * material.glass_roughness
                ).normalize();
            }

            ray.origin = record.point + refracted_dir * 0.001f;
            ray.direction = refracted_dir;

            refractive_index = n2;

            if (!record.front_face) {
                ray_color *= material.tint;
            }

            continue;
        }

        // Direct light sample for diffuse/non-glass surface
        std::shared_ptr<Object> light_obj = sample_light_source();

        if (light_obj) {
            Vec3 point_on_light = light_obj->sample_point_on_surface(record.point);

            Vec3 to_light = point_on_light - record.point;
            float distance_to_light = to_light.magnitude();
            Vec3 light_dir = to_light.normalize();

            float n_dot_l = std::max( 0.0f, static_cast<float>(record.normal * light_dir)
            );

            if (n_dot_l > 0.0f) {
                Ray direct_calc_ray( record.point + record.normal * 0.001f, light_dir
                );

                HitRecord light_hit = calculate_ray_collision(direct_calc_ray);

                if (light_hit.hit && light_hit.obj_id == light_obj->id) {
                    Color emitted_light =
                        light_hit.material.emission_color *
                        light_hit.material.emission_strength;

                    // Diffuse direct lighting
                    float diffuse_weight = 1.0f - material.specular_probability;

                    Color diffuse_direct = emitted_light * material.color * n_dot_l * diffuse_weight;

                    // Specular direct lighting
                    float specular_weight = material.specular_probability;
                    Color specular_direct(0.0f, 0.0f, 0.0f);

                    if (specular_weight > 0.0f) {
                        // View direction: from hit point back toward where the ray came from
                        Vec3 view_dir = (-incoming_dir).normalize();

                        // Perfect reflected direction of incoming light
                        Vec3 reflected_light_dir = (-light_dir).reflect(record.normal).normalize();

                        // How close is the view direction to the perfect reflection?
                        float alignment = std::max(0.0f, static_cast<float>(reflected_light_dir * view_dir)
                        );

                        // reflectivity controls how tight the highlight is:
                        // 0.0 -> broad / loose
                        // 1.0 -> extremely tight / almost mirror
                        float min_alignment = material.smoothness;

                        float spec_amount = 0.0f;

                        if (min_alignment >= 0.999f) {
                            spec_amount = (alignment > 0.999f) ? 1.0f : 0.0f;
                        } else if (alignment > min_alignment) {
                            spec_amount = (alignment - min_alignment) / (1.0f - min_alignment);
                        }

                        specular_direct =
                            emitted_light * material.specular_color * spec_amount * specular_weight;
                    }

                    Color direct_light = diffuse_direct + specular_direct;

                    incoming_light += direct_light * ray_color;
                }
            }
        }
        // Normal diffuse/specular bounce
        Vec3 diffuse_dir = random_in_hemisphere(record.normal).normalize();
        Vec3 reflect_dir = ray.direction.normalize().reflect(record.normal).normalize();

        bool is_specular_reflection = random_float() < material.specular_probability;

        if (is_specular_reflection) {
            ray.direction = (
                diffuse_dir * (1.0f - material.smoothness) +
                reflect_dir * material.smoothness
            ).normalize();

            ray_color *= material.specular_color;
        } else {
            ray.direction = diffuse_dir;
            ray_color *= material.color;
        }

        ray.origin = record.point + ray.direction * 0.001f;
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

                Vec3 focal_point = camera.get_position() + dir * camera.focal_distance;
                
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
                Vec3 new_origin = camera.get_position() + Vec3(dx, dy, 0);

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

void Environment::update_camera_position(const Vec3 position) {
    camera.update_position(position);
}

void Environment::update_camera_settings(float aperture, float focal_distance) {
    camera.aperture = aperture;
    camera.focal_distance = focal_distance;
}