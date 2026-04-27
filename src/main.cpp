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



void initialize_scene() {
    // Very matte / chalky materials
    Material matte_white(Color(0.9f, 0.9f, 0.9f), 0.0f, 0.0f);
    Material matte_black(Color(0.03f, 0.03f, 0.03f), 0.0f, 0.0f);
    Material matte_gray(Color(0.45f, 0.45f, 0.45f), 0.0f, 0.0f);
    Material matte_yellow(Color(1.0f, 0.85f, 0.15f), 0.0f, 0.0f);
    Material matte_purple(Color(0.45f, 0.15f, 0.8f), 0.0f, 0.0f);
    Material matte_red(Color(1.0f, 0.04f, 0.04f), 0.0f, 0.0f);

    // Satin / soft plastic
    Material satin_blue(Color(0.1f, 0.25f, 1.0f), 0.35f, 0.15f);
    Material satin_orange(Color(1.0f, 0.35f, 0.05f), 0.35f, 0.15f);
    Material satin_teal(Color(0.0f, 0.8f, 0.75f), 0.35f, 0.15f);
    Material satin_cyan(Color(0.15f, 0.95f, 1.0f), 0.35f, 0.25f);

    // Glossy colored plastic
    float gloss_r = 0.72f;
    float gloss_sp = 0.35f;
    Material glossy_red(Color(0.95f, 0.12f, 0.08f), gloss_r, gloss_sp);
    Material glossy_green(Color(0.12f, 0.92f, 0.18f), gloss_r, gloss_sp);
    Material glossy_blue(Color(0.08f, 0.22f, 0.95f), gloss_r, gloss_sp);
    Material glossy_pink(Color(0.95f, 0.22f, 0.62f), gloss_r, gloss_sp);
    Material glossy_purple(Color(0.42f, 0.14f, 0.78f), gloss_r, gloss_sp);
    Material glossy_lime(Color(0.35f, 0.95f, 0.12f), gloss_r, gloss_sp);
    Material glossy_white(Color(0.92f, 0.92f, 0.94f), gloss_r, gloss_sp);

    // Metallic-ish materials
    Material metallic_red(Color(1.0f, 0.05f, 0.05f), 0.5f, 0.85f);
    Material gold(Color(1.0f, 0.72f, 0.25f), 0.95f, 0.85f);
    Material copper(Color(0.95f, 0.45f, 0.2f), 0.9f, 0.75f);
    Material chrome(Color(0.9f, 0.9f, 0.94f), 1.0f, 1.0f);

    // Pure mirror
    Material mirror(Color(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);

    // Dark shiny material
    Material glossy_black(Color(0.02f, 0.02f, 0.025f), 0.9f, 0.8f);

    // Ground materials
    Material rough_ground(Color(0.8f, 0.8f, 0.78f), 0.05f, 0.02f);
    Material polished_ground(Color(0.72f, 0.72f, 0.74f), 0.05f, 0.02f);

    Material glass(Color(1.0f,1.0f,1.0f),0.3f,0.2f);
    glass.transparency_probability = 1;
    glass.refractive_index = 1.5f;

    Material light(Color(0.8f, 0.8f,0.8f), 0.1f, 1.0f);
    light.emission_color = Color(1.0f, 1.0f, 1.0f);
    light.emission_strength = 1;

    environment.add_object(std::make_shared<Sphere>(
        Vec3(0.0f, -10000.0f, 0.0f), 10000.0f, glossy_white
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(-900, 750, 0), 500.0f, light
    ));


    // glass.refractive_index = 10.0f;
    // glass.transparency_probability = 0.95f;
    environment.add_object(std::make_shared<Sphere>(
        Vec3(0.0f, 0.75f, 0.0f), 0.75f, matte_red
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(3.5f, 1.8f, -2.3f), 1.8f, mirror
    ));

    glossy_lime.emission_color = glossy_lime.color;
    glossy_lime.emission_strength = 2.5f;
    environment.add_object(std::make_shared<Sphere>(
        Vec3(-1.4f, 0.7f, -2.8f), 0.7f, glossy_lime
    ));

    environment.add_object(std::make_shared<Sphere>(
        Vec3(-4.5f, 1.9f, -6.0f), 1.9f, glossy_purple
    ));

    glossy_purple.emission_color = glossy_purple.color;
    glossy_purple.emission_strength = 1.2f;
    environment.add_object(std::make_shared<Sphere>(
        Vec3(1.2f, 0.35f, 1.3f), 0.35f, glossy_purple
    ));

    glass.refractive_index = 2.6f;
    environment.add_object(std::make_shared<Sphere>(
        Vec3(-1.5f, 0.65f, 1.3f), 0.65f, glass
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