#pragma once
#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

#include <thread>
#include <vector>
#include <stdio.h>
#include <execution>
#include "HittableList.h"
#include "Camera.h"
#include "Util.h"

#define WIDTH 1280
#define HEIGHT 720

const auto noThreads = std::thread::hardware_concurrency();
const auto maxSize = (WIDTH * HEIGHT) / noThreads;


struct args {
	int start;
	int end;
	uint32_t* pixels;
	HittableList scene;
	Camera cam;
};

glm::vec3 inline frag(Ray ray, const HittableList scene);


struct ThreadPool {
	std::vector<uint32_t> width;
	std::vector<uint32_t> height;
	uint32_t* pixels = { 0 };

	ThreadPool() { width.resize(WIDTH); height.resize(HEIGHT); pixels = new uint32_t[WIDTH * HEIGHT];}

	inline uint32_t* createThread(HittableList& scene, uint32_t*& imageData, Camera& cam) {
		pixels = new uint32_t[WIDTH * HEIGHT];
		memset(pixels, 0, WIDTH * HEIGHT * sizeof(uint32_t));

		std::for_each(std::execution::par, height.begin(), height.end(),
			[this, cam, imageData, scene](uint32_t y)
			{
				std::for_each(std::execution::par, width.begin(), width.end(),
				[this, y, cam, imageData, scene](uint32_t x) {
						for (int i = 0; i < 5; i++)
						{
							glm::vec3 col(0);
							glm::vec2 coord = { (float)x / (float)WIDTH, (float)y / (float)HEIGHT };
							coord = coord * 2.0f - 1.0f;

							Ray ray = cam.ray(coord.x, coord.y);

							col = frag(ray, scene);

							pixels[x + y * WIDTH] = BU::Color(col, 5);
						}

				});
			});

		return pixels;
	}

	void inline reap() {
		return;
	}



};


glm::vec3 frag(Ray ray, const HittableList scene) {
	hitRecord rec;					//holds our collision info
	glm::vec3 col(0);				//final color of the pixel
	glm::vec3 lightDir(-10);		//direction of incoming lightsource
	lightDir = glm::normalize(lightDir);
	float multiplier = 1.0f;		//to account for loss of energy with light
	constexpr float inf = std::numeric_limits<float>::infinity();
	int bounces = 2;

	for (int i = 0; i < bounces; i++)
	{
		if (scene.hit(ray, 0.00001, inf, rec)){
			Material mat = rec.mat;
			glm::vec3 normal = rec.normal;

			//get the color from the current hit object
			float intensity = glm::max(glm::dot(normal, -lightDir), 0.0f);
			glm::vec3 sphereCol = mat.colorVec();
			sphereCol *= intensity;
			col += sphereCol * multiplier;
			multiplier *= 0.5f;

			//generate the bounce based on smoothness of material
			glm::vec3 diff = BU::Diffuse(normal);
			glm::vec3 spec = BU::reflect(ray.getDirection(), normal);
			glm::vec3 dir = BU::lerp(diff, spec, mat.smoothness);

			//set the ray to new point and direction
			ray.origin(rec.p + normal * 1e-5f); ray.dir(dir);

		}
		else {	//generate a ambient sky color
			float t = 0.5 * ray.getDirection().y + 1;
			glm::vec3 sky = (1 - t) * glm::vec3(1) + t * glm::vec3(0.5, .7f, 1);
			col += sky * multiplier;
			break;
		}
	}
	return col;
}


#endif // !MULTIPROCESS_H
