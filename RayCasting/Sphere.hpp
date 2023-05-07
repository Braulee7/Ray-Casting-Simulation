#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Hittable.hpp"

struct Sphere : public Hittable {
	Sphere(glm::vec3& center, float radius, glm::vec3& col, float smoothness = 0) :mCenter(center), mRadius(radius) {
		mMat = Material(col, smoothness);
	}
	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec) const override;

	glm::vec3 mCenter;
	float mRadius;
};

bool Sphere::hit(const Ray& r, float min, float max, hitInfo& rec) const  {
	
	glm::vec3 oc = r.mOrigin - mCenter;

	float a = glm::dot(r.mDirection, r.mDirection);
	float b = glm::dot(oc, r.mDirection);
	float c = glm::dot(oc, oc) - (mRadius * mRadius);

	float disc = (b * b) - a * c;

	if (disc < 0) return false;

	float t = (-b - std::sqrt(disc)) / a;

	if (t < min || max < t) {
		t = (-b + std::sqrt(disc)) / a;
		if (t < min || max < t)
			return false;
	}

	rec.t = t;
	rec.hitPoint = (r.mOrigin - mCenter) + r.mDirection * t;
	rec.normal = glm::normalize(rec.hitPoint);
	rec.hitPoint += mCenter;
	rec.mat = mMat;
	return true;
}
#endif // !SPHERE_HPP
