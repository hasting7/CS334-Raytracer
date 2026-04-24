#include "Object.h"
#include "Hit.h"
#include "Ray.h"

#include <cmath>
#include <utility>


/*
Sphere has:
	- Vec3 position
	- Material material
	- float radius
Ray has:
	- Vec3 origin
	- Vec3 dir
*/

Hit Sphere::intersect(const Ray& ray) const {
	Hit hit_data = Hit();

	// first use the root test

	// quadratic formula values
	float a = ray.dir * ray.dir;
	float b = (ray.dir * -2.0f) * (position - ray.origin);
	float c = ((position - ray.origin) * (position - ray.origin)) - (radius*radius);

	float inside = (b*b) - (4.0f*a*c);

	if (inside < 0.0f) {
		hit_data.hit = false;
		return hit_data;
	}
	hit_data.hit = true;

	// finish
	float sol1 = (-b - sqrt(inside)) / (2.0f * a); // minus
	float sol2 = (-b + sqrt(inside)) / (2.0f * a); // plus

	if (sol1 > sol2) std::swap(sol1, sol2);

	float t;
	if (sol1 > 0.0f) {
	    t = sol1;
	} else if (sol2 > 0.0f) {
	    t = sol2;
	} else {
	    // both behind camera
	    hit_data.hit = false;
	    return hit_data;
	}
	hit_data.t = t;
	hit_data.position = ray.at(t);
	hit_data.material = material;
	hit_data.normal = (hit_data.position - position).normalize();

	return hit_data;
}