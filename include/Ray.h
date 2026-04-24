#pragma once


#include "Vec3.h"

struct Ray {
	Ray (const Vec3& origin, const Vec3& direction) : origin(origin), dir(direction) {};
	Ray() : Ray(Vec3(), Vec3()) {};
	Vec3 origin;
	Vec3 dir;

	Vec3 at(float t) const;
};