#pragma once

#include "Ray.h"
#include "Material.h"

struct HitRecord {
    float t;
    Vec3 point;
    Vec3 normal;
    Material material;
};

class Object {
public:
    virtual ~Object() = default;
    
    // Abstract hit method for intersection logic
    virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
};