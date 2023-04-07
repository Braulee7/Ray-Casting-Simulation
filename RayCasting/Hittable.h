#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include <glm.hpp>

struct hitRecord {
	glm::vec3 p;
	glm::vec3 normal;
	float t;
	bool faceNormal;

	void setNormal(const Ray& r, const glm::vec3& outwardNormal);
};

class Hittable {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, hitRecord& rec) const = 0;
};

#endif // !HITTABLE_H