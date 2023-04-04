#include "Sphere.h"


Sphere::Sphere(glm::vec3 center, float radius)
{
	mCenter = center;
	mRadius = radius;
}

float Sphere::getX() const
{
	return mCenter.x;
}

float Sphere::getY() const
{
	return mCenter.y;
}

float Sphere::getZ() const
{
	return mCenter.z;
}

float Sphere::radius() const
{
	return mRadius;
}

bool Sphere::hit(const Ray r) const
{
	//(bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby azbz))t + (ax^2 + ay^2 + az^2 - r^2)
	//         a                         b                           c
	
	glm::vec3 rayDir = r.getDirection();
	glm::vec3 rayOri = r.getOrigin();

	glm::vec3 oc = rayOri - mCenter;
	float a = glm::dot(rayDir, rayDir);
	float b = 2 * glm::dot(oc, rayDir);
	float c = glm::dot(oc, oc) - (mRadius * mRadius);

	float discriminant = (b * b) - 4 * a * c;
	return discriminant >= 0;
}
