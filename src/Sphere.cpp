#include "Sphere.h"
#include <cmath>

bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& rec) const {
    Vec3 oc = ray.origin - center;
    float a = ray.direction * ray.direction;
    float b = 2.0f * (oc * ray.direction);
    float c = (oc * oc) - radius * radius;
    
    float discriminant = b * b - 4.0f * a * c;
    
    if (discriminant > 0.0f) {
        // Find the nearest root that lies in the acceptable range.
        float root = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (root < t_max && root > t_min) {
            rec.t = root;
            rec.point = ray.at(rec.t);
            rec.normal = (rec.point - center).normalize();
            rec.material = material;
            return true;
        }
        
        // Second root check
        root = (-b + std::sqrt(discriminant)) / (2.0f * a);
        if (root < t_max && root > t_min) {
            rec.t = root;
            rec.point = ray.at(rec.t);
            rec.normal = (rec.point - center).normalize();
            rec.material = material;
            return true;
        }
    }
    return false;
}