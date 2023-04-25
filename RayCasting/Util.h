#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <glm.hpp>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <random>


namespace BU {

	float inline random() {
		srand(SDL_GetTicks());
		return rand();
	}

	float inline random(float min, float max) {
		std::uniform_real_distribution<float> dist(0, 1);
		std::default_random_engine gen((unsigned int)SDL_GetTicks());

		return dist(gen);
	}

	glm::vec3 inline randomDir(float min, float max) {
		float x = random(min, max);
		float y = random(min, max);
		float z = random(min, max);

		return glm::vec3(x, y, z);
	}

	bool inline nearZero(glm::vec3& vec) {
		const auto s = 1e-5;
		return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
	}

	template <typename T>
	T inline clamp(T val) {
		T min = (val < 255) ? val : 255;
		return (0 < min) ? min : 0;
	}

	glm::vec3 inline reflect(glm::vec3&& in, glm::vec3& normal) {
		glm::vec3 i = glm::normalize(in);
		return i - 2.0f * normal * glm::dot(i, normal);
		
	}

	glm::vec3 inline lerp(glm::vec3& a, glm::vec3& b, float t) {
		return a + (b - a) * t;
	}

	glm::vec3 inline normalize(glm::vec3& num) {
		float x = num.x / 255;
		float y = num.y / 255;
		float z = num.z / 255;

		return glm::vec3(x, y, z);
	}

	uint32_t inline Color(glm::vec3& vec, int samples) {

		float r = (vec.x);
		float g = (vec.y);
		float b = (vec.z);

		auto scale = 1.0 / samples;
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);


		//clamp values
		uint8_t red = (uint8_t)((255 * r));
		uint8_t green = (uint8_t)((255 * g));
		uint8_t blue = (uint8_t)((255 * b));

		//std::cout << red << '\t' << green << '\t' << blue << std::endl;
		return 0xff000000 | (red << 16) | (green << 8) | (blue);
	}

	glm::vec3 inline Diffuse(glm::vec3 &normal) {
		glm::vec3 vec(0);
		
		vec = glm::normalize(normal + randomDir(-1, 1));
		if (nearZero(vec)) return normal;

		return vec;
	}


}

#endif // !UTIL_H
