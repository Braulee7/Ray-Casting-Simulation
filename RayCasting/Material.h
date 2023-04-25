#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm.hpp>
#include "Util.h"

class Material {
public:
	Material() : mColor(1), lightCol(255), smoothness(0), lightStrength(0) {};
	Material(glm::vec3& color, float strength = 10, float light = 255, float smooth = 0);
	uint32_t getColor();
	glm::vec3 colorVec();
	inline glm::vec3 lightColor() { return BU::normalize(lightCol); };
	inline float lightStr() { return lightStrength; };
	void setColor(glm::vec3& color);
	float smoothness;

private:
	glm::vec3 mColor;
	glm::vec3 lightCol;
	float lightStrength = 0;
	

};

#endif // !MATERIAL_H
