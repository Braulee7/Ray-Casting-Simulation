#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <math.h>
#include <SDL.h>
#include "Ray.h"


class Camera {
public:
	Camera() = default;
	Camera(glm::vec3& pos, glm::vec3& front, glm::vec3& up, float fov, float aspectRatio);
	Ray ray(float x, float y);

private:

	glm::vec3 mOrigin;
	glm::vec3 horizontal;
	glm::vec3 vertical;

};


#endif