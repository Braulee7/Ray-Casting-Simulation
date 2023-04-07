#include "Camera.h"

Camera::Camera(glm::vec3& pos, glm::vec3& front, glm::vec3& up, float fov, float aspectRatio)
	:mOrigin(pos)
{
	float theta = glm::radians(fov);
	float h = tan(theta / 2);
	float vh = 2 * h;
	float vw = aspectRatio * vh;

	glm::vec3 w = glm::normalize(pos - front);
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = cross(w, u);


	vertical = glm::vec3(v.x * vh, v.y * vh, v.z * vh);
	horizontal = glm::vec3(u.x * vw, u.y * vw, u.z * vw);


}

Ray Camera::ray(float x, float y)
{
	return Ray(mOrigin, x*horizontal + y*vertical - mOrigin);
}

