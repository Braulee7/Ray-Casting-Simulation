#ifndef SPHERE_H
#define SPHERE_H

#include <glm.hpp> 
#include "Ray.h"
#include "Hittable.h"

class Sphere : public Hittable{
public:
	Sphere(glm::vec3 center, float radius);
	float getX() const;
	float getY() const;
	float getZ() const;
	float radius() const;
	virtual bool hit(const const Ray &r, float tMin, float tMax, hitRecord &rec) const override;
private:
	glm::vec3 mCenter;
	float mRadius;
};

#endif // !SPHERE_H
