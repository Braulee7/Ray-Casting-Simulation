#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <glm.hpp>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <random>


namespace BU {

	//generates a random number based on ticks 
	//since app started as a seed
	float inline random() {
		srand(SDL_GetTicks());
		return rand();
	}

	//Generates a random uniform number based on a mean
	//and standard deviation. This allows for a more 
	//distributed set of numbers (better for spheres)
	float inline random(float mean, float stdev) {
		std::uniform_real_distribution<float> dist(0, 1);
		std::default_random_engine gen((unsigned int)SDL_GetTicks());

		return dist(gen);
	}


	//Generates a vector with a random number which will
	//be inside the sphere
	glm::vec3 inline randomDir(float mean, float stdev) {
		float x = random(mean, stdev);
		float y = random(mean, stdev);
		float z = random(mean, stdev);

		return glm::vec3(x, y, z);
	}

	//checks if the given vector is near zero for 
	//floating point numbers
	bool inline nearZero(glm::vec3& vec) {
		const auto s = 1e-5;
		return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
	}

	//clamps a given value between 0 and 255
	//generalised for most numerical primitive
	//types
	template <typename T>
	T inline clamp(T val) {
		T min = (val < 255) ? val : 255;
		return (0 < min) ? min : 0;
	}

	//Refelct function to mimic perfect reflect angles
	//for a given vector
	//args:		
	//		in: The intersection ray
	//		normal: The normal ray from the point of intersection
	glm::vec3 inline reflect(glm::vec3& in, glm::vec3& normal) {
		glm::vec3 i = glm::normalize(in);
		return i - 2.0f * normal * glm::dot(i, normal);
		
	}

	//Lerp function to return a vector between a and b
	//based on paramater t
	glm::vec3 inline lerp(glm::vec3& a, glm::vec3& b, float t) {
		return a + (b - a) * t;
	}

	//normalises a vector to be between 0 and 1
	//instead of 0 and 255 for rgb values
	glm::vec3 inline normalize(glm::vec3& num) {
		float x = num.x / 255;
		float y = num.y / 255;
		float z = num.z / 255;

		return glm::vec3(x, y, z);
	}

	//
	uint32_t inline Color(glm::vec3& vec, int samples) {

		//gamma correction
		auto scale = 1.0 / samples;
		float r = sqrt(scale * vec.r);
		float g = sqrt(scale * vec.g);
		float b = sqrt(scale * vec.b);

		//get to 255 rgb value
		uint8_t red = (uint8_t)((255 * r));
		uint8_t green = (uint8_t)((255 * g));
		uint8_t blue = (uint8_t)((255 * b));

		return 0xff000000 | (red << 16) | (green << 8) | (blue);
	}

	//Generates a diffuse direction for rougher 
	//materials, direction goes off in a random
	//direction from the point of impact/normal
	glm::vec3 inline Diffuse(glm::vec3 &normal) {
		glm::vec3 vec(0);
		
		vec = glm::normalize(normal + randomDir(-1, 1));
		if (nearZero(vec)) return normal;

		return vec;
	}


}

#endif // !UTIL_H
