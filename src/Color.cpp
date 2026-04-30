#include "Color.h"
#include <algorithm>
#include <stdexcept>

Color::Color() : r(0.0f), g(0.0f), b(0.0f) {}

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

uint32_t Color::to_int() const {
    float cr = std::clamp(r, 0.0f, 1.0f);
    float cg = std::clamp(g, 0.0f, 1.0f);
    float cb = std::clamp(b, 0.0f, 1.0f);

    cr = std::sqrt(cr);
    cg = std::sqrt(cg);
    cb = std::sqrt(cb);

    return 0xFF000000
        | ((static_cast<uint32_t>(cr * 255.999f) & 0xFF) << 16)
        | ((static_cast<uint32_t>(cg * 255.999f) & 0xFF) << 8)
        |  (static_cast<uint32_t>(cb * 255.999f) & 0xFF);
}


Color Color::operator+(const Color& other) const {
    return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator-(const Color& other) const {
    return Color(r - other.r, g - other.g, b - other.b);
}

Color Color::operator*(const Color& other) const {
    return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::operator/(const Color& other) const {
    return Color(r / other.r, g / other.g, b / other.b);
}


Color Color::operator+(float value) const {
    return Color(r + value, g + value, b + value);
}

Color Color::operator-(float value) const {
    return Color(r - value, g - value, b - value);
}

Color Color::operator*(float value) const {
    return Color(r * value, g * value, b * value);
}

Color Color::operator/(float value) const {
    return Color(r / value, g / value, b / value);
}

Color& Color::operator+=(const Color& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Color& Color::operator-=(const Color& other) {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
}

Color& Color::operator*=(const Color& other) {
    r *= other.r;
    g *= other.g;
    b *= other.b;
    return *this;
}

Color& Color::operator/=(const Color& other) {
    r /= other.r;
    g /= other.g;
    b /= other.b;
    return *this;
}


Color& Color::operator+=(float value) {
    r += value;
    g += value;
    b += value;
    return *this;
}

Color& Color::operator-=(float value) {
    r -= value;
    g -= value;
    b -= value;
    return *this;
}

Color& Color::operator*=(float value) {
    r *= value;
    g *= value;
    b *= value;
    return *this;
}

Color& Color::operator/=(float value) {
    r /= value;
    g /= value;
    b /= value;
    return *this;
}


Color operator+(float value, const Color& color) {
    return Color(value + color.r, value + color.g, value + color.b);
}

Color operator-(float value, const Color& color) {
    return Color(value - color.r, value - color.g, value - color.b);
}

Color operator*(float value, const Color& color) {
    return Color(value * color.r, value * color.g, value * color.b);
}