#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>
#include <memory>
#include <ctime>

#include "Environment.h"
#include "Vec3.h"
#include "Sphere.h"

static const int width = 800;
static const int height = 600;

Environment environment = Environment(width, height, 12);

// 

void initialize_scene() {
    // Mat params: Color, Ambient, Diffuse, Reflectivity
    Material glossy_red(Color(1.0f, 0.0f, 0.0f), 0.2f, 0.4f);
    Material glossy_green(Color(0.0f, 1.0f, 0.0f), 0.2f, 0.4f);
    Material mirror(Color(1,1,1),1.0f,0.98f);

    Material blue_light(Color(0,0,1.0f),0.3f,0.2f);
    blue_light.emission_color = Color(0,0,1);
    blue_light.emission_strength = 1.2f;

    Material ground(Color(1.0f, 1.0f, 1.0f), 1.0f, 0.2f);
    // mat_blue.emission_strength = 0.5f;
    // mat_blue.emission_color = Color(0,0,1);

    Material light(Color(0.8f, 0.8f,0.8f), 0.1f, 1.0f);
    light.emission_color = Color(1.0f, 1.0f, 1.0f);
    light.emission_strength = 1;

    // 3 spheres at increasing depth from the camera
    // Front sphere
    environment.add_object(std::make_shared<Sphere>(
        Vec3(8, 4, -8), 4.0f, glossy_green
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(0, 3, -10), 3.0f, mirror
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(-8, 4, -8), 4.0f, glossy_red
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(0, -1000.0f, 0), 1000.0f, ground
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(0, 1.0f, -0.5f), 1.0f, blue_light
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(0, 1100, 0), 750.0f, light
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(-100, 20, 0), 50.0f, light
    ));

    // Lights
    environment.add_light(Light(Vec3(0.0f, 15.0f, 10.0f), 1.0f));
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