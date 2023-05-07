#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "Material.hpp"
#include "Ray.hpp"

struct hitInfo {
	glm::vec3 hitPoint;
	glm::vec3 normal;
	float t;
	Material mat;

	hitInfo() : hitPoint(0), normal(0), t(0) {};
};

struct Hittable {

	Hittable() = default;
	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec) const = 0;

	Material mMat;

};

#endif
