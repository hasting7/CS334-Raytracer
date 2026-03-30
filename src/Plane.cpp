#include "Object.h"
#include "Hit.h"
#include "Ray.h"


/*
Plane has:
	- Vec3 position
	- Material material
	- Vec3 normal
Ray has:
	- Vec3 origin
	- Vec3 dir
*/

Hit Plane::intersect(const Ray& ray) const {
	return Hit();
}