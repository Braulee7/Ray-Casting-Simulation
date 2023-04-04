#ifndef RAY_H
#define RAY_H

#include <glm.hpp>

class Ray {
public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	glm::vec3 at(float t) const;

private:
	glm::vec3 mOrigin;
	glm::vec3 mDirection;
};
#endif // !RAY_H
