#ifndef TRIANGLE_HPP
#define TRIANLGE_HPP

#include "Hittable.hpp"

struct Triangle : Hittable {
	glm::vec3 a, b, c;				//coords for each point of tri
	glm::vec3 normA, normB, normC;	//normals for each popint of tri

	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec) const override;

};


bool Triangle::hit(const Ray& r, float min, float max, hitInfo& rec) const {
	glm::vec3 BA = b - a;
	glm::vec3 CA = c - a;
	glm::vec3 pvec = glm::cross(r.mDirection, CA);
	float det = glm::dot(BA, pvec);

	if (fabs(det) < FLT_EPSILON) return false;

	float invDet = 1 / det;
	glm::vec3 tvec = r.mOrigin - a;
	
	float u = glm::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	glm::vec3 qvec = glm::cross(tvec, BA);
	float v = glm::dot(r.mDirection, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	glm::vec3 normalVec = glm::cross(BA, CA);


	float t = glm::dot(CA, qvec) * invDet;

	rec.t = t;
	rec.mat = mMat;


	return true;


}
#endif // !TRIANGLE_HPP
