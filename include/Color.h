#pragma once

#include <cstdint>

class Color {
public:
    float r, g, b;

    Color();
    Color(float r, float g, float b);

    uint32_t to_int() const;

    // Color with Color
    Color operator+(const Color& other) const;
    Color operator-(const Color& other) const;
    Color operator*(const Color& other) const;   // component-wise multiply
    Color operator/(const Color& other) const;   // component-wise divide

    // Color with float
    Color operator+(float value) const;
    Color operator-(float value) const;
    Color operator*(float value) const;
    Color operator/(float value) const;

    // Compound assignment with Color
    Color& operator+=(const Color& other);
    Color& operator-=(const Color& other);
    Color& operator*=(const Color& other);
    Color& operator/=(const Color& other);

    // Compound assignment with float
    Color& operator+=(float value);
    Color& operator-=(float value);
    Color& operator*=(float value);
    Color& operator/=(float value);
};

// float on left side
Color operator+(float value, const Color& color);
Color operator-(float value, const Color& color);
Color operator*(float value, const Color& color);