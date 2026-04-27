#pragma once

#include "Ray.h"
#include "Material.h"

struct HitRecord {
    bool hit = false;
    double t = 1e9f;
    Vec3 point;
    Vec3 normal;
    Material material;
    bool front_face;
};

class Object {
public:
    Object(Material mat) : material(mat) {};
    virtual ~Object() = default;
    
    Material material;
    // Abstract hit method for intersection logic
    virtual bool hit(const Ray& ray, double t_min, HitRecord& rec) const = 0;
};