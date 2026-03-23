cmake_minimum_required(VERSION 3.16)
project(cs334_raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3)

add_executable(cs334_raytracer
    src/main.cpp
)

target_link_libraries(cs334_raytracer PRIVATE SDL3::SDL3)