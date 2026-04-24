#include "Ray.h"
#include "Vec3.h"

Vec3 Ray::at(float t) const {
	return origin + (dir * t);
}
