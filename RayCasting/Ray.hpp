#ifndef RAY_HPP
#define RAY_HPP

#include <glm.hpp>

struct Ray {
	Ray(glm::vec3 origin, glm::vec3 direction) : mOrigin(origin), mDirection(glm::normalize(direction)) {};

	glm::vec3 mOrigin;
	glm::vec3 mDirection;
};
#endif // !RAY_HPP
