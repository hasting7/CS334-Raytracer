#include "Vec3.h"

#include <stdio.h>
#include <cassert>
#include <cmath>

void Vec3::visualize() {
	printf("Vec3 (x = %.2f, y = %.2f, z = %.2f)\n", x,y,z);
}

Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

// ADDITION RULES

Vec3& Vec3::operator+=(const Vec3 &other) { // FIXED: Added const
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3& Vec3::operator+=(double value) {
	this->x += value;
	this->y += value;
	this->z += value;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &other) const {
	return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3 Vec3::operator+(double value) const {
	return Vec3(this->x + value, this->y + value, this->z + value);
}

// SUBTRACTION RULES

Vec3& Vec3::operator-=(const Vec3 &other) { // FIXED: Added const
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vec3& Vec3::operator-=(double value) {
	this->x -= value;
	this->y -= value;
	this->z -= value;
	return *this;
}

Vec3 Vec3::operator-(const Vec3 &other) const {
	return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3 Vec3::operator-(double value) const {
	return Vec3(this->x - value, this->y - value, this->z - value);
}

// MULTIPLICATION RULES

Vec3& Vec3::operator*=(double value) {
	this->x *= value;
	this->y *= value;
	this->z *= value;
	return *this;
}

Vec3 Vec3::operator*(double value) const {
	return Vec3(this->x * value, this->y * value, this->z * value);
}




// DIVISION RULES

Vec3& Vec3::operator/=(double value) {
	assert(value != 0.0f && "VEC3 DIVIDE BY ZERO ERROR");
	this->x /= value;
	this->y /= value;
	this->z /= value;
	return *this;
}

Vec3 Vec3::operator/(double value) const {
	assert(value != 0.0f && "VEC3 DIVIDE BY ZERO ERROR");
	return Vec3(this->x / value, this->y / value, this->z / value);
}

Vec3 operator+(double value, const Vec3& v) {
    return Vec3(value + v.x, value + v.y, value + v.z);
}

Vec3 operator-(double value, const Vec3& v) {
    return Vec3(value - v.x, value - v.y, value - v.z);
}

Vec3 operator*(double value, const Vec3& v) {
    return Vec3(value * v.x, value * v.y, value * v.z);
}

Vec3 operator/(double value, const Vec3& v) {
    return Vec3(value / v.x, value / v.y, value / v.z);
}

// EXTRA PROPERTIES

double Vec3::operator*(const Vec3& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

double Vec3::magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
    double mag = magnitude();
    assert(mag != 0.0f && "CANNOT NORMALIZE A ZERO VECTOR");
    return *this / mag; 
}

double Vec3::distance(const Vec3& a, const Vec3& b) {
    return (a - b).magnitude();
}

Vec3 Vec3::reflect(const Vec3& n) const {
    // R = V - 2(V dot N)N
    return *this - n * (2.0f * (*this * n));
}

Vec3 random_vec3() {
    static thread_local std::mt19937 rng(std::random_device{}());
    static thread_local std::uniform_real_distribution<double> dist(-1.0f, 1.0f);
    return Vec3(dist(rng), dist(rng), dist(rng));
}

Vec3 random_in_hemisphere(const Vec3& normal) {
	Vec3 v;
    while (true) {
        v = random_vec3();
        double len_sq = v.x * v.x + v.y * v.y + v.z * v.z;

        // reject zero vector and anything outside unit sphere
        if (len_sq <= 1e-8f || len_sq > 1.0f) {
            continue;
        }

        // normalize
        double inv_len = 1.0f / std::sqrt(len_sq);
        v = v * inv_len;

        // flip if it is on the wrong hemisphere
        if (v * normal < 0.0f) {
            v = v * -1.0f;
        }

        return v;
    }
}