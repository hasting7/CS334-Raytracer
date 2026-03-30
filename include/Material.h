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
    Color color{255, 255, 255};
};