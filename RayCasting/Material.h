#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>


class Material {
public:
	Material() : mColor(0) {};
	Material(glm::vec3 color);
	uint32_t getColor();
	void setColor(glm::vec3 color);

private:
	glm::vec3 mColor;
};

#endif // !MATERIAL_H
