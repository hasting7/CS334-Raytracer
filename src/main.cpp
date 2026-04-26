#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>
#include <memory>
#include <ctime>

#include "Environment.h"
#include "Vec3.h"
#include "Sphere.h"

static const int width = 400;
static const int height = 300;

Environment environment = Environment(width, height, 12);

// 

void initialize_scene() {
    // Mat params: Color, Ambient, Diffuse, Reflectivity
    // Lower reflectivity makes blur easier to judge visually
    Material mat_red(Color(255, 60, 60), 0.1f, 0.9f, 0.0f);
    Material mat_green(Color(60, 255, 60), 0.1f, 0.9f, 0.0f);
    Material mat_blue(Color(60, 60, 255), 0.1f, 0.9f, 0.0f);
    Material mat_floor(Color(240, 240, 240), 0.1f, 0.9f, 0.0f);

    // 3 spheres at increasing depth from the camera
    // Front sphere
    environment.add_object(std::make_shared<Sphere>(
        Vec3(-2.0f, 0.0f, -3.0f), 1.0f, mat_red
    ));

    // Middle sphere
    environment.add_object(std::make_shared<Sphere>(
        Vec3(0.0f, 0.0f, -5.0f), 1.0f, mat_blue
    ));

    // Back sphere
    environment.add_object(std::make_shared<Sphere>(
        Vec3(2.0f, 0.0f, -7.0f), 1.0f, mat_green
    ));

    // Floor
    environment.add_object(std::make_shared<Sphere>(
        Vec3(0.0f, -101.0f, -5.0f), 100.0f, mat_floor
    ));

    // Lights
    environment.add_light(Light(Vec3(0.0f, 8.0f, 2.0f), 1.0f));
    // environment.add_light(Light(Vec3(-6.0f, 4.0f, 0.0f), 0.4f));
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