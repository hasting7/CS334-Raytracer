#pragma once

#include "Color.h"

struct Material {
    Color color;
    Color emission_color;
    float emission_strength = 0.0f;
    float reflectivity; // NEW: 0.0 is matte, 1.0 is a perfect mirror
    float specular_probability;
    float refractive_index = 0;
    float transparency_probability = 0.0f;


    Material() : color(Color(255, 255, 255)), reflectivity(0.0f), specular_probability(1.0f) {}
    
    // Updated constructor with a default value for reflectivity so old code doesn't break
    Material(Color c, float r, float sp) : color(c), reflectivity(r), specular_probability(sp) {}
};

// Very matte / chalky materials
inline Material matte_white(Color(0.9f, 0.9f, 0.9f), 0.0f, 0.0f);
inline Material matte_black(Color(0.03f, 0.03f, 0.03f), 0.0f, 0.0f);
inline Material matte_gray(Color(0.45f, 0.45f, 0.45f), 0.0f, 0.0f);
inline Material matte_yellow(Color(1.0f, 0.85f, 0.15f), 0.0f, 0.0f);
inline Material matte_purple(Color(0.45f, 0.15f, 0.8f), 0.0f, 0.0f);
inline Material matte_red(Color(1.0f, 0.04f, 0.04f), 0.0f, 0.0f);

// Satin / soft plastic
inline Material satin_blue(Color(0.1f, 0.25f, 1.0f), 0.35f, 0.15f);
inline Material satin_orange(Color(1.0f, 0.35f, 0.05f), 0.35f, 0.15f);
inline Material satin_teal(Color(0.0f, 0.8f, 0.75f), 0.35f, 0.15f);
inline Material satin_cyan(Color(0.15f, 0.95f, 1.0f), 0.35f, 0.25f);

// Glossy colored plastic
inline Material glossy_red(Color(0.95f, 0.12f, 0.08f), 0.72f, 0.35f);
inline Material glossy_green(Color(0.12f, 0.92f, 0.18f), 0.72f, 0.35f);
inline Material glossy_blue(Color(0.08f, 0.22f, 0.95f), 0.72f, 0.35f);
inline Material glossy_pink(Color(0.95f, 0.22f, 0.62f), 0.72f, 0.35f);
inline Material glossy_purple(Color(0.42f, 0.14f, 0.78f), 0.72f, 0.35f);
inline Material glossy_lime(Color(0.35f, 0.95f, 0.12f), 0.72f, 0.35f);
inline Material glossy_white(Color(0.92f, 0.92f, 0.94f), 0.72f, 0.35f);
inline Material glossy_black(Color(0.02f, 0.02f, 0.025f), 0.9f, 0.8f);

// Metallic-ish materials
inline Material metallic_red(Color(1.0f, 0.05f, 0.05f), 0.5f, 0.85f);
inline Material gold(Color(1.0f, 0.72f, 0.25f), 0.95f, 0.85f);
inline Material copper(Color(0.95f, 0.45f, 0.2f), 0.9f, 0.75f);
inline Material chrome(Color(0.9f, 0.9f, 0.94f), 1.0f, 1.0f);

// Pure mirror
inline Material mirror(Color(1.0f, 1.0f, 1.0f), 1.0f, 1.0f);