#ifndef HITTABLE_H
#define HITTABLE_H

#include <glm.hpp>
#include "Ray.h"
#include "Material.h"

struct hitRecord {
	glm::vec3 p;
	glm::vec3 normal;
	Material mat;
	float t;
	bool faceNormal;

	void setNormal(const Ray& r, const glm::vec3& outwardNormal);

	inline hitRecord() : p(0), normal(0), mat(glm::vec3(0)), t(0), faceNormal(false) {};
};

class Hittable {
public:
	Hittable() = default;
	virtual bool hit(const Ray& r, float t_min, float t_max, hitRecord& rec) const = 0;
	virtual void setColor(glm::vec3 color);
	Material mMat;
};

#endif // !HITTABLE_H
