#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm.hpp>

struct Material {
	Material() = default;
	Material(glm::vec3& color, float smoothness = 0) : mColor(color), mSmoothness(smoothness){}

	glm::vec3 mColor;
	float mSmoothness;
};
#endif // !MATERIAL_HPP
