#include "Ray.h"
#include <iostream>

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
{
	mOrigin = origin;
	mDirection = direction;
}


glm::vec3 Ray::getOrigin() const
{
	return mOrigin;
}

glm::vec3 Ray::getDirection() const
{
	return mDirection;
}

glm::vec3 Ray::at(float t) const
{
	//since t is a scalar, we need to multiply each of 
	//the coords with t to get bt
	glm::vec3 val = glm::vec3(mDirection.x * t, mDirection.y * t, mDirection.z * t);
	
	//a + bt
	return mOrigin + val;
}
