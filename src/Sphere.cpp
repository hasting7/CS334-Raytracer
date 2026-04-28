#include "Sphere.h"
#include <cmath>

bool Sphere::hit(const Ray& ray, double t_min, HitRecord& rec) const {
    Vec3 oc = ray.origin - center;

    double a = ray.direction * ray.direction;
    double b = 2.0 * (oc * ray.direction);
    double c = (oc * oc) - radius * radius;

    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0) {
        return false;
    }

    double sqrt_d = std::sqrt(discriminant);

    double root = (-b - sqrt_d) / (2.0 * a);

    if (root <= t_min || root >= rec.t) {
        root = (-b + sqrt_d) / (2.0 * a);

        if (root <= t_min || root >= rec.t) {
            return false;
        }
    }

    rec.t = root;
    rec.point = ray.at(rec.t);

    Vec3 outward_normal = (rec.point - center).normalize();

    rec.front_face = (ray.direction * outward_normal) < 0.0;
    rec.normal = rec.front_face ? outward_normal : -outward_normal;

    rec.material = material;
    rec.hit = true;

    return true;
}

Vec3 Sphere::sample_point_on_surface(const Vec3 origin) const {
    return Vec3();
}