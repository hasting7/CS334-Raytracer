#include "Color.h"
#include <cstdint>
#include <cassert>
#include <cmath>
#include <algorithm>

uint32_t Color::to_uint32() const {
    // Reinhard tonemap
    float tr = r / (r + 1.0f);
    float tg = g / (g + 1.0f);
    float tb = b / (b + 1.0f);

    // gamma correction
    tr = std::sqrt(tr);
    tg = std::sqrt(tg);
    tb = std::sqrt(tb);

    uint8_t ri = (uint8_t)(std::min(std::max(tr, 0.0f), 1.0f) * 255.0f);
    uint8_t gi = (uint8_t)(std::min(std::max(tg, 0.0f), 1.0f) * 255.0f);
    uint8_t bi = (uint8_t)(std::min(std::max(tb, 0.0f), 1.0f) * 255.0f);

    return 0xFF000000 | (ri << 16) | (gi << 8) | bi;
}

Color Color::to_color(uint32_t color_int) {
    return Color(((color_int >> 16) & 0xFF) / 255.0f, ((color_int >> 8) & 0xFF) / 255.0f, (color_int & 0xFF) / 255.0f);
}

Color Color::operator*(float value) const {
    return Color(r * value, g * value, b * value);
}

Color Color::operator+(const Color& other) const {
    return Color(r + other.r, g + other.g, b + other.b);
}

Color& Color::operator+=(const Color& other) {
    this->r += other.r;
    this->g += other.g;
    this->b += other.b;
    return *this;
}

Color operator*(float value, const Color& c) {
    return c * value;
}
Color Color::operator*(const Color& other) const {
    return Color(r * other.r, g * other.g, b * other.b);
}

Color Color::operator/(float value) const {
    assert(value != 0.0f && "COLOR DIVIDE BY ZERO ERROR");
    return Color(this->r / value, this->g / value, this->b / value);
}

Color& Color::operator/=(float value) {
    assert(value != 0.0f && "COLOR DIVIDE BY ZERO ERROR");
    this->r /= value;
    this->g /= value;
    this->b /= value;
    return *this;
}