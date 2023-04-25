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
	//a + bt
	return mOrigin + mDirection * t;
}
