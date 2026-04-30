#pragma once

#include "Object.h"
#include "Vec3.h"
#include "Material.h"

class Triangle : public Object {
public:
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, Material material)
        : v0(v0), v1(v1), v2(v2), Object(material) {}

    bool hit(const Ray& ray, double t_min, HitRecord& rec) const override;
    Vec3 sample_point_on_surface(const Vec3 origin) const override;

    Vec3 v0, v1, v2;
};