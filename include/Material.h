#pragma once

#include "Color.h"

struct Material {
    Color color;
    Color emission_color;
    float emission_strength = 0.0f;
    float reflectivity;             // [0,1] rough, mirror like
    float specular_probability;     // 
    Color specular_color = Color(1.0f,1.0f,1.0f);
    float refractive_index = 1.5f; 
    float transmission = 0.0f;      // {0, non 0} 0 for not glass, 1 if glass
    float glass_roughness = 0.0f;   // [0, 1] (tapers around 0.3) clear glass to rough glass
    Color tint;



    Material() : color(Color(255, 255, 255)), reflectivity(0.0f), specular_probability(1.0f) {}
    
    // Updated constructor with a default value for reflectivity so old code doesn't break
    Material(Color c, float r, float sp) : color(c), reflectivity(r), specular_probability(sp) {}
};

