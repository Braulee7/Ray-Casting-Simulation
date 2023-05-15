#ifndef TRIANGLE_HPP
#define TRIANLGE_HPP

#include "Hittable.hpp"

struct Triangle : Hittable {
	glm::vec3 a, b, c;				//coords for each point of tri
	glm::vec3 normA, normB, normC;	//normals for each popint of tri

	Triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 col, float smoothness = 0)
	: a(pointA), b(pointB), c(pointC){

		normA = glm::cross((b - a), (c - a));
		normB = glm::cross((a - b), (c - b));
		normC = glm::cross((a- c), (b - c));

		mMat = Material(col, smoothness);
	}
	
	Triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 normalA, glm::vec3 normalB, glm::vec3 normalC, glm::vec3 col, float smoothness = 0) 
	: a(pointA), b(pointB), c(pointC), normA(normalA), normB(normalB), normC(normalC) {
		mMat = Material(col, smoothness);
	}
	virtual bool hit(const Ray& r, float min, float max, hitInfo& rec) const override;

};


bool Triangle::hit(const Ray& r, float min, float max, hitInfo& rec) const {
	
	glm::vec3 e1, e2, pvec, qvec, tvec, normal;

	e1 = b - a;
	e2 = c - a;
	pvec = glm::cross(r.mDirection, e2);
	normal = glm::cross(e1, e2);

	glm::vec3 n = glm::normalize(r.mDirection);
	//NORMALIZE(pvec);
	float det = glm::dot(pvec, e1);

	if (det != 0)
	{
		float invDet = 1.0f / det;
		tvec = r.mOrigin - a;

		float t = glm::dot(tvec, normal) * invDet;
		if (t < min || max < t) return false;

		// NORMALIZE(tvec);
		float u = invDet * glm::dot(tvec, pvec);
		if (u < 0.0f || u > 1.0f)
		{

			return false;
		}
		qvec = glm::cross(tvec, e1);
		// NORMALIZE(qvec);
		float v = invDet * glm::dot(qvec, n);
		if (v < 0.0f || u + v > 1.0f)
		{

			return false;
		}

		float w = 1 - u - v;

		rec.hitPoint = t * r.mDirection + r.mOrigin;
		rec.t = t;
		rec.normal = glm::normalize(normA * w + normB * u + normC * v);
		rec.mat = mMat;
	}
	else return false;

	return true;


	/*
	//edges of triangle
	glm::vec3 BA = b - a;
	glm::vec3 CA = c - a;
	glm::vec3 normalVec = glm::cross(BA, CA);
	
	// origin - pointA
	glm::vec3 ao = r.mOrigin - a;
	glm::vec3 pvec = glm::cross(r.mDirection, CA);
	float det = glm::dot(pvec, BA);

	//if the det is close to zero then the 
	//ray is parallel with the triangle and misses
	if (det < 1E-6) return false;

	//invert the determinant since we are going
	//to be dividing with it
	float invDet = 1 / det;

	float t = glm::dot(ao, normalVec) * invDet;

	if (t < min || max < t) return false;

	// (tvec ' pvec) / determinant
	float u = glm::dot(ao, pvec) * invDet;
	if (u < 0) return false;

	//glm::vec3 qvec = glm::cross(ao, BA);
	glm::vec3 qvec = glm::cross(ao, BA);
	float v = glm::dot(r.mDirection, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	float w = 1 - u - v;


	rec.t = t;
	rec.mat = mMat;
	rec.hitPoint = r.mOrigin + r.mDirection * t;
	rec.normal = glm::normalize(normA * w + normB * u + normC * v);


	return true;
	*/


}
#endif // !TRIANGLE_HPP
