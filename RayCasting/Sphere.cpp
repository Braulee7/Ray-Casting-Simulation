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

uint32_t Sphere::hit(const Ray r) const
{
	//(bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby azbz))t + (ax^2 + ay^2 + az^2 - r^2)
	//         a                         b                           c
	
	glm::vec3 rayDir = r.getDirection();
	glm::vec3 rayOri = r.getOrigin();

	glm::vec3 oc = rayOri - mCenter;
	float a = glm::dot(rayDir, rayDir);
	float halfB = glm::dot(oc, rayDir);
	float c = glm::dot(oc, oc) - (mRadius * mRadius);

	float discriminant = (halfB * halfB) - a * c;

	if (discriminant < 0)
		return 0xff000000;
	//Calculate the Normal

	float t = (-halfB - discriminant) / a;
	glm::vec3 pointOfIntersection = r.at(t);
	glm::vec3 normal = pointOfIntersection - mCenter;

	//get channels
	uint8_t red = (uint8_t) normal.x;
	uint8_t green = (uint8_t)normal.y;
	uint8_t blue = (uint8_t)normal.z;

	return 0xff000000 | (red << 16) | (green << 8) | blue;
}
