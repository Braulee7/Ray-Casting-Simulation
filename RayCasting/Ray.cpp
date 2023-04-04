#include "Ray.h"

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
	//t is a 1D object so we need to 
	//convert it to a vector to use in 
	//equation
	glm::vec3 T = glm::vec3(t);

	// a + bt
	return mOrigin + mDirection * T;
}
