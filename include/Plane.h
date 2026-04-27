#pragma once

#include "Object.h"

class Plane : public Object {
public:
    Plane(Vec3 center, Vec3 normal, Material mat) 
        : Object(mat), center(center), normal(normal) {}
        
    bool hit(const Ray& ray, double t_min, HitRecord& rec) const override;
    
    Vec3 center;
    Vec3 normal;
private:
};