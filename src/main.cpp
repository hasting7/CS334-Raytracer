#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>
#include <memory>
#include <ctime>

#include "Environment.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Material.h"

// ARGUMENTS

static const int   width = 800;
static const int   height = 600;
static const int   threads = 12;

static const int   max_ray_depth = 100;
static const int   rays_per_pixel = 10;

static const Vec3  camera_origin = Vec3(0, 2.5, 8);
static const float aperture = 0.3;
static const float focal_distance = 8;

Environment environment = Environment(width, height, threads, max_ray_depth, rays_per_pixel);

void make_square(Vec3 bottom_left, Vec3 bottom_right, Material mat);

void initialize_scene() {
    // set camera settings
    environment.update_camera_position(camera_origin);
    environment.update_camera_settings(aperture, focal_distance);

    Material floor(Color(0.4f, 0.4f, 0.4f), 1.0f, 0.18f);
    environment.add_object(std::make_shared<Plane>(
        Vec3(0, 0, 0), Vec3(0, 1, 0), floor
    ));

    Material light(Color(1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
    light.emission_strength = 1.0f;
    light.emission_color = Color(0.99f, 0.99f, 0.99f);
    environment.add_object(std::make_shared<Sphere>(
        Vec3(-40, 40, 20), 8, light
    ));

    // add objects 

    Material glossy_white(Color(1,1,1), 0.9,0.5);
    Material glossy_red(Color(1,0.05,0.05), 0.9,0.5);
    Material glossy_blue(Color(0.05,0.05,1), 0.9,0.5);
    // white ball
    environment.add_object(std::make_shared<Sphere>(
        Vec3(0, 2, 0), 2, glossy_white
    ));

    // red balls
    // big red ball
    environment.add_object(std::make_shared<Sphere>(
        Vec3(7, 4, -2), 4, glossy_red
    ));

    // small red ball
    environment.add_object(std::make_shared<Sphere>(
        Vec3(-5.5, 1.5, -1.5), 1.5, glossy_red
    ));

    // back blue ball
    environment.add_object(std::make_shared<Sphere>(
        Vec3(3, 4, -9), 4, glossy_blue
    ));
}


void make_square(Vec3 bottom_left, Vec3 bottom_right, Material mat) {
    Vec3 width_vec = bottom_right - bottom_left;
    float side_length = width_vec.magnitude();

    Vec3 top_left = bottom_left + Vec3(0.0f, side_length, 0.0f);
    Vec3 top_right = bottom_right + Vec3(0.0f, side_length, 0.0f);

    environment.add_object(std::make_shared<Triangle>(
        bottom_left,
        bottom_right,
        top_right,
        mat
    ));

    environment.add_object(std::make_shared<Triangle>(
        bottom_left,
        top_right,
        top_left,
        mat
    ));
}

int main(int argc, char* argv[]) {
    initialize_scene();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("Raytracer", width, height, 0, &window, &renderer)) {
        SDL_Log("CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );



    // Render the scene exactly ONCE. (With AA and reflections, this takes a few seconds)
    time_t start_time, end_time;
    time(&start_time);
    printf("Rendering...\n");
    environment.render();
    time(&end_time);
    double elapsed_time = difftime(end_time, start_time);
    printf("Render complete! (%.2f seconds)\n", elapsed_time);
    
    if (!SDL_UpdateTexture(texture, nullptr, environment.framebuffer.data(), width * sizeof(uint32_t))) {
        SDL_Log("UpdateTexture failed: %s", SDL_GetError());
    }

    bool running = true;
    SDL_Event e;

    int frame_count = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        if (frame_count % 1 == 0) {
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }

        frame_count++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}