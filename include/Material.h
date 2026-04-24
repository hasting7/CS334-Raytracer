#pragma once

#include "Color.h"

/*

all material properties should go here once we know what we want to do
- color
- surface texture
- glossy?
- if it emits light / what color
- shinny

*/

struct Material {
    Color color{1.0f, 1.0f, 1.0f};
    Color emission{0.0f,0.0f,0.0f};
};