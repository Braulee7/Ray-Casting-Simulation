#include "Hittable.h"

void hitRecord::setNormal(const Ray& r, const glm::vec3& outwardNormal) {
	faceNormal = glm::dot(r.getDirection(), outwardNormal) > 0;
	normal = faceNormal ? -outwardNormal : outwardNormal;
}

//hittable class, will be overriden by all inheritance
bool Hittable::hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const
{
	return false;
}

void Hittable::setColor(glm::vec3 color) {
	mMat.setColor(color);
}

