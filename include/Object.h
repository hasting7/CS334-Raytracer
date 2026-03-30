#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Hit.h"


class Object {
public:
	Object(const Vec3& position, const Material& material) : position(position), material(material) {};
	virtual ~Object() = default;
	virtual Hit intersect(const Ray& ray) const = 0;
private:
	Vec3 position;
	Material material;
};

class Sphere : public Object {
public:
	Sphere(const Vec3& position, const Material& material, float radius) : Object(position, material), radius(radius) {};

	Hit intersect(const Ray& ray) const override;
private:
	float radius;
};


class Plane : public Object {
public:
	Plane(const Vec3& position, const Material& material, const Vec3& normal) : Object(position, material), normal(normal) {};

	Hit intersect(const Ray& ray) const override;
private:
	Vec3 normal;
};

