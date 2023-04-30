#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <vector>
#include <string>
#include <glm.hpp>
#include <random>
#include "HittableList.h"
#include "Camera.h"

class Image {
public:
	Image();
	~Image();

	void Init(const int x, const int y, SDL_Renderer* renderer, HittableList& scene, Camera &cam);
	void Render();
	void InitTexture();


private:
	//height and width of the image
	uint32_t mWidth, mHeight;
	//the data of the image
	uint32_t *mImageData;
	//SDL2 rendering stuff to see in real time
	//the image
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;

	//iterators for multithreading loops
	std::vector<uint32_t> mWidthIt, mHeightIt;
	
	//holds final color of individual pixels
	glm::vec3 col;

	HittableList mScene;
	Camera mCam;

	glm::vec3 fragShader(uint32_t x, uint32_t y);

};

#endif
