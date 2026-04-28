#pragma once

#include "Object.h"

class Sphere : public Object {
public:
    Sphere(Vec3 center, float radius, Material mat) 
        : Object(mat), center(center), radius(radius) {}
        
    bool hit(const Ray& ray, double t_min, HitRecord& rec) const override;
    Vec3 sample_point_on_surface(const Vec3 origin) const override;

    Vec3 center;
    float radius;
private:
};