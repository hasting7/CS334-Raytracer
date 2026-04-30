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
    int obj_id;
};

class Object {
private:
    inline static int id_iterator = 0;
public:
    Object(Material mat) : material(mat), id(Object::id_iterator++) {};
    virtual ~Object() = default;
    
    Material material;
    int id;
    // Abstract hit method for intersection logic
    virtual bool hit(const Ray& ray, double t_min, HitRecord& rec) const = 0;
    virtual Vec3 sample_point_on_surface(const Vec3 origin) const = 0;
};