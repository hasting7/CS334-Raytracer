#include "Vec3.h"

#include <stdio.h>
#include <cassert>
#include <cmath>
#include <random>


void Vec3::visualize() {
	printf("Vec3 (x = %.2f, y = %.2f, z = %.2f)\n", x,y,z);
}

Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

// ADDITION RULES

Vec3& Vec3::operator+=(Vec3 &other) {
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3& Vec3::operator+=(float value) {
	this->x += value;
	this->y += value;
	this->z += value;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &other) const {
	return Vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vec3 Vec3::operator+(float value) const {
	return Vec3(this->x + value, this->y + value, this->z + value);
}

// SUBTRACTION RULES

Vec3& Vec3::operator-=(Vec3 &other) {
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	return *this;
}

Vec3& Vec3::operator-=(float value) {
	this->x -= value;
	this->y -= value;
	this->z -= value;
	return *this;
}

Vec3 Vec3::operator-(const Vec3 &other) const {
	return Vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vec3 Vec3::operator-(float value) const {
	return Vec3(this->x - value, this->y - value, this->z - value);
}

// MULTIPLICATION RULES

Vec3& Vec3::operator*=(float value) {
	this->x *= value;
	this->y *= value;
	this->z *= value;
	return *this;
}

Vec3 Vec3::operator*(float value) const {
	return Vec3(this->x * value, this->y * value, this->z * value);
}

// DIVISION RULES

Vec3& Vec3::operator/=(float value) {
	assert(value != 0.0f && "VEC3 DIVIDE BY ZERO ERROR");
	this->x /= value;
	this->y /= value;
	this->z /= value;
	return *this;
}

Vec3 Vec3::operator/(float value) const {
	assert(value != 0.0f && "VEC3 DIVIDE BY ZERO ERROR");
	return Vec3(this->x / value, this->y / value, this->z / value);
}

// EXTRA PROPERTIES

// vec3 * vec3
float Vec3::operator*(const Vec3& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

// vec3.magnitude
float Vec3::magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

// Vec3::distance(vec3 v1, vec3 v2)
float Vec3::distance(const Vec3& a, const Vec3& b) {
    return (a - b).magnitude();
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
	return Vec3((a.y * b.z) - (a.z * b.y), (a.x * b.z) - (a.z * b.x), (a.x * b.y) - (a.y * b.x));
}

Vec3 Vec3::normalize() const {
	float mag = magnitude();
	assert(mag != 0.0f && "CANNOT NORMALIZE ZERO VECTOR");
	return *this / mag;
}

#include <random>
#include <cmath>

Vec3 random_hemisphere_dir(const Vec3& normal) {
    static thread_local std::mt19937 rng(std::random_device{}());
    static thread_local std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    while (true) {
        float x = dist(rng);
        float y = dist(rng);
        float z = dist(rng);

        Vec3 v(x, y, z);
        float len_sq = v.x * v.x + v.y * v.y + v.z * v.z;

        // reject zero vector and anything outside unit sphere
        if (len_sq <= 1e-8f || len_sq > 1.0f) {
            continue;
        }

        // normalize
        float inv_len = 1.0f / std::sqrt(len_sq);
        v = v * inv_len;

        // flip if it is on the wrong hemisphere
        if (v * normal < 0.0f) {
            v = v * -1.0f;
        }

        return v;
    }
}