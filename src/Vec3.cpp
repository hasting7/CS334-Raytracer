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

float Vec3::operator*(const Vec3& other) const {
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

float Vec3::magnitude() const {
	return std::sqrt(x * x + y * y + z * z);
}

float Vec3::distance(const Vec3& a, const Vec3& b) {
    return (a - b).magnitude();
}