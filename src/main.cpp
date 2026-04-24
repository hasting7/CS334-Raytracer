#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>

#include "Environment.h"
#include "Vec3.h"
#include "Object.h"

static const int width = 400;
static const int height = 300;

Environment environment = Environment(width,height);

void initialize_scene() {
    Material ball_a;
    ball_a.color = Color(0.85f, 0.25f, 0.15f); // warm red

    Material ball_b;
    ball_b.color = Color(0.2f, 0.45f, 0.9f); // cool blue
    ball_b.emission = Color(0.0f, 0.05f, 0.0f); 

    Material light;
    light.emission = Color(90.0f, 80.5f, 70.0f); // warm white, bright

    // big red ball, sitting low
    environment.add_object(std::make_unique<Sphere>(Vec3(-12.0f, -1.0f, 30.0f), ball_a, 10.0f));

    // smaller blue ball, sitting higher
    environment.add_object(std::make_unique<Sphere>(Vec3(10.0f, 1.5f, 28.0f), ball_b, 8.0f));

    // light ball — centered between them, slightly above and closer to camera
    environment.add_object(std::make_unique<Sphere>(Vec3(-0.2f, 2.0f, -50.0f), light, 2.0f));
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

    if (!texture) {
        SDL_Log("CreateTexture failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    std::vector<uint32_t> framebuffer(width * height, 0xFF000000); // opaque black

    bool running = true;
    SDL_Event e;

    int frame_count = 0;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        environment.render(framebuffer);

        if (!SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t))) {
            SDL_Log("UpdateTexture failed: %s", SDL_GetError());
            break;
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