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

static const int width = 800;
static const int height = 600;

Environment environment = Environment(width, height, 12);

void make_square(Vec3 bottom_left, Vec3 bottom_right, Material mat);

void initialize_scene() {
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

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}