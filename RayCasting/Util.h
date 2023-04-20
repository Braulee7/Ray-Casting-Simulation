#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <glm.hpp>

namespace BU {
	uint32_t inline Color(glm::vec3 vec, int samples) {
		float r = (vec.x + 1);
		float g = (vec.y + 1);
		float b = (vec.z + 1);

		auto scale = 1.0 / samples;
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);

		uint8_t red = (uint8_t)(255.0f * r);
		uint8_t green = (uint8_t)(255.0f * g);
		uint8_t blue = (uint8_t)(255.0f * b);

		return 0xff000000 | (red << 16) | (green << 8) | (blue);
	}
}

#endif // !UTIL_H
