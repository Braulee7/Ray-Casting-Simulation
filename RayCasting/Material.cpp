#include "Material.h"

Material::Material(glm::vec3& color)
	:mColor(color)
{
}

uint32_t Material::getColor()
{
	uint32_t r = (uint8_t)(0.5 * ((mColor.x + 1)));;
	uint32_t g = (uint8_t)(0.5 * ((mColor.y + 1)));
	uint32_t b = (uint8_t)(0.5 * ((mColor.z + 1)));
	
	return 0xff000000 | (r << 16) | (g << 8) | (b);
}

glm::vec3 Material::colorVec() {
	return mColor;
}

void Material::setColor(glm::vec3& color)
{
	mColor = color;
}
