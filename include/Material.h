#pragma once

#include "Color.h"

struct Material {
    Color color;
    float ambient;
    float diffuse;
    float reflectivity; // NEW: 0.0 is matte, 1.0 is a perfect mirror

    Material() : color(Color(255, 255, 255)), ambient(0.1f), diffuse(0.9f), reflectivity(0.0f) {}
    
    // Updated constructor with a default value for reflectivity so old code doesn't break
    Material(Color c, float a, float d, float r = 0.0f) : color(c), ambient(a), diffuse(d), reflectivity(r) {}
};