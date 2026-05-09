cmake_minimum_required(VERSION 3.16)
project(cs334_raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3)

add_executable(cs334_raytracer
    src/main.cpp
    src/Environment.cpp
    src/Camera.cpp
    src/Vec3.cpp
    src/Color.cpp
    src/Sphere.cpp
    src/Plane.cpp
    src/Triangle.cpp
    src/Ray.cpp
)

target_include_directories(cs334_raytracer PRIVATE include)

target_link_libraries(cs334_raytracer PRIVATE SDL3::SDL3)