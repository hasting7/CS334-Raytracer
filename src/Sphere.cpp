#include "Object.h"
#include "Hit.h"
#include "Ray.h"


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
	return Hit();
}