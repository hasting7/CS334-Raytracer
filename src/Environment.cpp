#include <vector>

#include "Environment.h"
#include "Camera.h"
#include "Color.h"
#include "Vec3.h"
#include "Ray.h"

#include <random>


Environment::Environment(int width, int height) : width(width), height(height) {
	camera = Camera(Vec3(0.0f,0.0f,-15.0f), Vec3(0.0f,0.0f,1.0f), 60.0f, width, height);
	ray_depth = 10;
	rays_per_pixel = 3;
	alpha = 0.9f;
	frame_count = 0;

	std::mt19937_64 gen(rd()); 
	std::uniform_real_distribution<float> uniform_dist(0.0f, 1.0f); 

	for (int i = 0; i < width * height; i++) {
		color_history.push_back(Color());
	}
}

void Environment::add_object(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

void Environment::render(std::vector<uint32_t> &framebuffer) {
    frame_count++;
    float weight = 1.0f / (float)frame_count;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color final_color;
            float step = 1.0f / static_cast<float>(rays_per_pixel);
            for (int x_inc = 0; x_inc < rays_per_pixel; x_inc++) {
                for (int y_inc = 0; y_inc < rays_per_pixel; y_inc++) {
                    float x_off = (x_inc + 0.5f) * step;
                    float y_off = (y_inc + 0.5f) * step;
                    Ray view_ray = camera.make_ray(x, y, x_off, y_off);
                    final_color += shoot_ray(view_ray, ray_depth, 5, true);
                }
            }
            float total_rays = static_cast<float>(rays_per_pixel * rays_per_pixel);
            final_color /= total_rays;

            if (frame_count == 1) {
                color_history[y * width + x] = final_color;
            } else {
                color_history[y * width + x] = color_history[y * width + x] * (1.0f - weight) + final_color * weight;
            }
            framebuffer[y * width + x] = color_history[y * width + x].to_uint32();
        }
    }
}

Color Environment::shoot_ray(const Ray& ray, int depth, int branch, bool from_camera) {
    if (depth <= 0) return Color(0.0f, 0.0f, 0.0f);
    Hit nearest_hit;
    for (const auto& obj : objects) {
        Hit hit_data = obj->intersect(ray);
        if (hit_data.hit && (!nearest_hit.hit || hit_data.t < nearest_hit.t))
            nearest_hit = hit_data;
    }
    if (nearest_hit.hit) {
        Color emitted = nearest_hit.material.emission;
        Color bounced_total;
        for (int i = 0; i < branch; i++) {
            Vec3 bounce_dir = random_hemisphere_dir(nearest_hit.normal);
            Ray bounce_ray(nearest_hit.position + nearest_hit.normal * 0.001f, bounce_dir);
            // only branch on first bounce, subsequent bounces stay at 1
            // otherwise cost is branch^depth which gets insane fast
            Color bounced_color = shoot_ray(bounce_ray, depth - 1, 1, false);
            float cos_theta = bounce_dir * nearest_hit.normal;
            bounced_total += bounced_color * cos_theta * 2.0f;
        }
        bounced_total = bounced_total / (float)branch;
        return emitted + nearest_hit.material.color * bounced_total;
    }
	// if (from_camera) {
	//     float t = (ray.dir.y + 1.0f) * 0.5f;
	//     return Color(0.5f, 0.7f, 1.0f) * t + Color(1.0f, 1.0f, 1.0f) * (1.0f - t);
	// }
    return Color(0.0f, 0.0f, 0.0f);
}