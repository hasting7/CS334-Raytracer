#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <cstdint>

#include "Environment.h"
#include "Vec3.h"
#include "Object.h"

static const int width = 800;
static const int height = 600;

Environment enviornment = Environment(width,height);

void initalize_scene() {
    // this is adding a red ball to the scene

    // idk if this is the best way to do it but for now
    Material red = Material();
    red.color = Color(255,0,0);

    enviornment.add_object(std::make_unique<Sphere>(Vec3(), red, 5.0));
    enviornment.add_point_light(Vec3(100,100,0));

}


int main(int argc, char* argv[]) {
    initalize_scene();


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
        enviornment.render(framebuffer);

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