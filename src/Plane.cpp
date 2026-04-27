#include "Plane.h"
#include <cmath>

bool Plane::hit(const Ray& ray, double t_min, HitRecord& rec) const {
    double denom = ray.direction * normal;

    if (std::fabs(denom) < 1e-8) {
        return false;
    }

    double t = ((center - ray.origin) * normal) / denom;

    if (t < t_min || (t > rec.t)) {
        return false;
    }

    rec.t = t;
    rec.point = ray.at(rec.t);
    rec.material = material;
    rec.hit = true;

    rec.front_face = denom < 0.0;

    rec.normal = rec.front_face ? normal : -normal;

    return true;
}