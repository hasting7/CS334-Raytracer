#pragma once

#include "Vec3.h"
#include "Material.h"

struct Hit
{
	bool hit = false;
	float t;
	Vec3 position;
	Vec3 normal;
	Material material;
};