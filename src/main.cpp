#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>

#include "Environment.h"
#include "Vec3.h"

static const int width = 800;
static const int height = 600;



int main(int argc, char* argv[]) {
    Vec3 a = Vec3(3,4,0);
    Vec3 b = Vec3(0,2,3);

    printf("a dot b: %.3f\n",Vec3::distance(a,b));


    a.visualize();
    // b.visualize();

    // Vec3 c = -a;
    // c.visualize();

    // Vec3 d = a / 0.01f;
    // d.visualize();
    return 0;

    Environment env = Environment();

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

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Example: set one pixel
        framebuffer[100 * width + 100] = 0xFFFF0000; // opaque red in ARGB8888

        if (!SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t))) {
            SDL_Log("UpdateTexture failed: %s", SDL_GetError());
            break;
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