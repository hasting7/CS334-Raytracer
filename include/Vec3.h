#pragma once

#include <random>

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x, double y) : x(x), y(y), z(0) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    
    void visualize();

    // subtraction properties
    Vec3 operator-() const;
    Vec3& operator-=(const Vec3& other); // FIXED: Added const
    Vec3 operator-(const Vec3& other) const;

	Vec3& operator-=(double value);
    Vec3 operator-(double value) const;

    // addtion properties
    Vec3& operator+=(const Vec3& other); // FIXED: Added const
    Vec3 operator+(const Vec3& other) const;

    Vec3& operator+=(double value);
    Vec3 operator+(double value) const;

    // multiplication properties
    Vec3& operator*=(double value);
    Vec3 operator*(double value) const;

    // division properties
    Vec3& operator/=(double value);
    Vec3 operator/(double value) const;

    // extra properties

    // dot product
    double operator*(const Vec3& other) const; 

    // magnitude
    double magnitude() const;
    
    // normalize (returns a unit vector)
    Vec3 normalize() const;

    // distance
    static double distance(const Vec3& a, const Vec3& b);
    
    // reflection
    Vec3 reflect(const Vec3& n) const;

    double x, y, z;
private:
};

// Float on left side
Vec3 operator+(double value, const Vec3& v);
Vec3 operator-(double value, const Vec3& v);
Vec3 operator*(double value, const Vec3& v);
Vec3 operator/(double value, const Vec3& v);

Vec3 random_in_hemisphere(const Vec3& normal);
Vec3 random_vec3();