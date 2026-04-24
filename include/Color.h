#pragma once

#include <cstdint>

struct Color {
    Color() : Color(0.0f, 0.0f, 0.0f) {}
    Color(float r, float g, float b) : r(r), g(g), b(b) {}

    float r, g, b;

    uint32_t to_uint32() const;
    static Color to_color(uint32_t color_int);

    Color operator*(float value) const;
    Color operator*(const Color& other) const;
    Color operator/(float value) const;
    Color& operator/=(float other);
    Color operator+(const Color& other) const;
    Color& operator+=(const Color& other);

};

Color operator*(float value, const Color& c);