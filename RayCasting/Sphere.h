#ifndef SPHERE_H
#define SPHERE_H

#include <glm.hpp> 
#include "Ray.h"

class Sphere {
public:
	Sphere(glm::vec3 center, float radius);
	float getX() const;
	float getY() const;
	float getZ() const;
	float radius() const;
	uint32_t hit(const Ray r) const;
private:
	glm::vec3 mCenter;
	float mRadius;
};

#endif // !SPHERE_H
