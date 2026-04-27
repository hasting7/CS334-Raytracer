#pragma once

#include "Color.h"

struct Material {
    Color color;
    Color emission_color;
    float emission_strength = 0.0f;
    float reflectivity; // NEW: 0.0 is matte, 1.0 is a perfect mirror
    float specular_probability;

    Material() : color(Color(255, 255, 255)), reflectivity(0.0f), specular_probability(1.0f) {}
    
    // Updated constructor with a default value for reflectivity so old code doesn't break
    Material(Color c, float r, float sp) : color(c), reflectivity(r), specular_probability(sp) {}
};