# CS334 Ray Tracer

A ray tracer built in C++ for CS334. Group project.

## Project Structure

- **`main.cpp`** — This is where scenes are constructed. Edit this file to build and configure scenes (camera, objects, lights, etc.).
- **`material.h`** — Contains the material class and an outline of all material properties available (albedo, roughness, metallic, etc.).

## Dependencies

| Dependency | Purpose |
|------------|---------|
| [SDL2](https://www.libsdl.org/) | Window creation and real-time render preview |
| [CMake](https://cmake.org/) | Build system (macOS) |

## Platform Support

| Platform | Support |
|----------|---------|
| macOS | ✅ Best supported |
| Windows | ⚠️ Additional environment setup required |

> **Windows users:** You'll need to configure your build environment manually (e.g. install CMake, a C++ compiler via MSYS2/MinGW or MSVC, and SDL2). Reach out if you need help getting set up.

## Building & Running (macOS)

```bash
# 1. Clone the repository
git clone <repo-url>
cd <repo-folder>

# 2. Create a build folder and run CMake
cmake -S . -B build

# 3. Build the project
cmake --build build

# 4. Run the ray tracer
./build/cs334_raytracer
```

The ray tracer renders in real-time to an SDL window. The image progressively refines as more samples are accumulated.

## Render Showcase

Sample renders demonstrating various features of the ray tracer. See [`RayTracer_image_collection.pdf`](RayTracer_image_collection.pdf) for the full collection.

**Figure 1** — 3 balls with different material properties: rough, glossy, tinted frosted glass

**Figure 2** — Material properties and depth of field

**Figure 3** — Reflections and triangle rendering with mirror walls

**Figure 4** — Depth of field with the glass ball in focus and other balls blurred

**Figure 5** — Colored light and caustics through frosted glass at varying frost levels

**Figure 6** — Emissive balls lighting a scene; focal plane set to the mirror reflection, leaving real objects out of focus

**Figure 7** — RGB colored light refracted through a glass ball

**Figure 8** — Scene before direct light sampling (noisy baseline comparison)

## Authors

Built for CS334 — group project.