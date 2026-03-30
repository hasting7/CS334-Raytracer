#pragma once

#include <cstdint>

struct Color {
    Color() : Color(0,0,0) {};
    Color(int r, int g, int b) : r(r), b(b), g(g) {};
    int r, g, b;
    uint32_t to_int() const;
};