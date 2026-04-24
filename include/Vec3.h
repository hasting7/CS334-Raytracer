#pragma once

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y) : x(x), y(y), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    void visualize();

    // subtraction properties
    Vec3 operator-() const;
    Vec3& operator-=(const Vec3& other); // FIXED: Added const
    Vec3 operator-(const Vec3& other) const;

	Vec3& operator-=(float value);
    Vec3 operator-(float value) const;

    // addtion properties
    Vec3& operator+=(const Vec3& other); // FIXED: Added const
    Vec3 operator+(const Vec3& other) const;

    Vec3& operator+=(float value);
    Vec3 operator+(float value) const;

    // multiplication properties
    Vec3& operator*=(float value);
    Vec3 operator*(float value) const;

    // division properties
    Vec3& operator/=(float value);
    Vec3 operator/(float value) const;

    // extra properties

    // dot product
    float operator*(const Vec3& other) const; 

    // magnitude
    float magnitude() const;
    
    // normalize (returns a unit vector)
    Vec3 normalize() const;

    // distance
    static float distance(const Vec3& a, const Vec3& b);
    
    // reflection
    static Vec3 reflect(const Vec3& v, const Vec3& n);

    float x, y, z;
private:
};