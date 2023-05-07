#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Ray.hpp"

class Camera {
public:
	Camera() = default;
	Camera(glm::vec3& pos, glm::vec3& front, glm::vec3& up, float fov, float aspectRatio)
		:mOrigin(pos)
	{
		float theta = glm::radians(fov);
		float h = tan(theta / 2);
		float vh = 2 * h;
		float vw = aspectRatio * vh;

		glm::vec3 w = glm::normalize(pos - front);
		glm::vec3 u = glm::normalize(glm::cross(up, w));
		glm::vec3 v = cross(w, u);


		mVertical = glm::vec3(v.x * vh, v.y * vh, v.z * vh);
		mHorizontal = glm::vec3(u.x * vw, u.y * vw, u.z * vw);

	}

	Ray ray(float x, float y) const { return Ray(mOrigin, x * mHorizontal + y * mVertical - mOrigin);; }
private:

	glm::vec3 mOrigin;
	glm::vec3 mHorizontal;
	glm::vec3 mVertical;
};
#endif // !CAMERA_HPP
