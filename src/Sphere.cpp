#include "Sphere.h"
#include <cmath>


bool Sphere::hit(const Ray& ray, double t_min, HitRecord& rec) const {
    Vec3 oc = ray.origin - center;
    double a = ray.direction * ray.direction;
    double b = 2.0f * (oc * ray.direction);
    double c = (oc * oc) - radius * radius;
    
    double discriminant = b * b - 4.0f * a * c;
    
    if (discriminant > 0.0f) {
        // Find the nearest root that lies in the acceptable range.
        double root = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (root < rec.t && root > t_min) {
            rec.t = root;
            rec.point = ray.at(rec.t);
            rec.normal = (rec.point - center).normalize();
            rec.material = material;
            rec.hit = true;
            rec.front_face = rec.normal * ray.direction < 0.0f;
            return true;
        }
        
        // Second root check
        root = (-b + std::sqrt(discriminant)) / (2.0f * a);
        if (root < rec.t && root > t_min) {
            rec.t = root;
            rec.point = ray.at(rec.t);
            rec.normal = (rec.point - center).normalize();
            rec.material = material;
            rec.front_face = rec.normal * ray.direction < 0.0f;
            rec.hit = true;
            return true;
        }
    }
    return false;
}