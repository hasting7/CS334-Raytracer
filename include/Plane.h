#pragma once

#include "Object.h"

class Plane : public Object {
public:
    Plane(Vec3 center, Vec3 normal, Material mat) 
        : Object(mat), center(center), normal(normal) {}
        
    bool hit(const Ray& ray, double t_min, HitRecord& rec) const override;
    Vec3 sample_point_on_surface(const Vec3 origin) const override;
    
    Vec3 center;
    Vec3 normal;
private:
};