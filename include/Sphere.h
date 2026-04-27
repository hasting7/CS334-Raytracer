#pragma once

#include "Object.h"

class Sphere : public Object {
public:
    Sphere(Vec3 center, float radius, Material mat) 
        : center(center), radius(radius), material(mat) {}
        
    bool hit(const Ray& ray, float t_min, HitRecord& rec) const override;
    
    Vec3 center;
    float radius;
private:

    Material material;
};