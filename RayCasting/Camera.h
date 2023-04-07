#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>
#include <math.h>
#include <SDL.h>


class Camera {
public:
	Camera();
	~Camera();

private:
	glm::vec3 mOrigin;
	glm::vec3 mForward;
	glm::vec3 mUp;
	glm::vec3 mDirection;

	bool mFirstClick;
	float mSpeed;
	float mSensitivity;
};


#endif