#pragma once

#include "Ray.h"
#include "Material.h"

struct HitRecord {
    bool hit = false;
    float t = 1e9f;
    Vec3 point;
    Vec3 normal;
    Material material;
};

class Object {
public:
    virtual ~Object() = default;
    
    // Abstract hit method for intersection logic
    virtual bool hit(const Ray& ray, float t_min, HitRecord& rec) const = 0;
};