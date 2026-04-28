#include "Triangle.h"

bool Triangle::hit(const Ray& ray, double t_min, HitRecord& rec) const {
    const float EPSILON = 1e-8f;

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 h = ray.direction.cross(edge2);
    float a = edge1 * h;

    if (std::fabs(a) < EPSILON) {
        return false;
    }

    float f = 1.0f / a;
    Vec3 s = ray.origin - v0;
    float u = f * (s * h);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 q = s.cross(edge1);
    float v = f * (ray.direction * q);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    float t = f * (edge2 * q);

    if (t <= t_min || t >= rec.t) {
        return false;
    }

    rec.t = t;
    rec.point = ray.at(t);

    Vec3 outward_normal = edge1.cross(edge2).normalize();

    rec.front_face = (ray.direction * outward_normal) < 0.0f;
    rec.normal = rec.front_face ? outward_normal : -outward_normal;

    rec.material = material;
    rec.hit = true;

    return true;
}

Vec3 Triangle::sample_point_on_surface(const Vec3 origin) const {
    return Vec3();
}