#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <glm.hpp>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

namespace BU {
	uint32_t inline Color(glm::vec3 vec, int samples) {
		float r = (vec.x);
		float g = (vec.y);
		float b = (vec.z);

		auto scale = 1.0 / samples;
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);


		//clamp values
		uint8_t red = (uint8_t)(256 * (r > 255.999 ? 255.999 : r < 0 ? 0 : r));
		uint8_t green = (uint8_t)(256 * (g > 255.999 ? 255.999 : g < 0 ? 0 : g));
		uint8_t blue = (uint8_t)(256 * (b > 255.999 ? 255.999 : b < 0 ? 0 : b));

		//std::cout << red << '\t' << green << '\t' << blue << std::endl;
		return 0xff000000 | (red << 16) | (green << 8) | (blue);
	}

	glm::vec3 Hemisphere(glm::vec3 &normal) {
		glm::vec3 vec(0);
		srand(SDL_GetTicks());

		vec = glm::vec3(rand() % (RAND_MAX + 1), rand() % (RAND_MAX + 1), rand() % (RAND_MAX + 1));
		float val = glm::length(glm::dot(vec, normal));

		return val > 0 ? vec : -vec;
	}
}

#endif // !UTIL_H
