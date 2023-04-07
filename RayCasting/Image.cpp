#include <iostream>
#include <cmath>
#include <limits>
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"


Image::Image()
{
	mTexture = nullptr;
	mWidth = 0;
	mHeight = 0;
	mImageData = nullptr;
	mRenderer = nullptr;
}

Image::~Image()
{
	if (mTexture)
		SDL_DestroyTexture(mTexture);
}

void Image::Init(const int x, const int y, SDL_Renderer* renderer)
{
	mWidth = x;
	mHeight = y;
	mRenderer = renderer;

	InitTexture();
}

void Image::InitTexture()
{
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
#endif

	if (mTexture)
		SDL_DestroyTexture(mTexture);

	SDL_Surface* tempSurface = SDL_CreateRGBSurface(0, mWidth, mHeight, 32, rmask, gmask, bmask, amask);
	mTexture = SDL_CreateTextureFromSurface(mRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}

void Image::Render(HittableList &scene, Camera &cam)
{
	//create a 1D array to hold the image information
	mImageData = new uint32_t[mWidth * mHeight];

	//clear data
	memset(mImageData, 0, mHeight * mWidth * sizeof(uint32_t));


	//for each pixel of the image send it to our "frag shader"
	for (uint32_t y = 0; y < mHeight; y++) {
		for (uint32_t x = 0; x < mWidth; x++) {

			//get coordinate of individual pixel
			glm::vec2 coord = { (float) x / (float) mWidth, (float) y / (float) mHeight };
			coord = coord * 2.0f - 1.0f;

			Ray ray = cam.ray(coord.x, coord.y);

			mImageData[x + y * mWidth] = fragShader(ray, scene);
		}
	}

	//show the image to the renderer
	SDL_UpdateTexture(mTexture, nullptr, mImageData, mWidth * sizeof(Uint32));

	//clear the image buffer
	delete[] mImageData;

	//present the image to the screen
	SDL_Rect src, bound;
	src.x = 0;
	src.y = 0;
	src.w = mWidth;
	src.h = mHeight;
	bound = src;
	SDL_RenderCopyEx(mRenderer, mTexture, &src, &bound, 0, NULL,  SDL_FLIP_VERTICAL);
	
}

//meant return a color for each pixel on the screen
uint32_t Image::fragShader(Ray &ray, HittableList& scene)
{
	//shoot a ray from the screen torward the pixel
	//Ray ray(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(coord.x, coord.y, -1.0f));

	hitRecord rec;

	if (scene.hit(ray, 0, std::numeric_limits<float>::infinity(), rec)) {
		return rec.mat.getColor();
	}
	else {
		glm::vec3 dir = ray.getDirection();

		auto unitDir = (dir) / glm::length(dir);
		float t = 0.5 * (unitDir.y + 1);

		uint8_t red	  =	(uint8_t)	((1 - t) * (1 * 255.999f)) + ((t) * (0.5 * 255.999f));
		uint8_t green = (uint8_t)	((1 - t) * (1 * 255.999f)) + ((t) * (0.7 * 255.999f));
		uint8_t blue  =	(uint8_t)	((1 - t) * (1 * 255.999f)) + ((t) * (1.0 * 255.999f));

		return 0xff000000 | (red << 16) | (green << 8) | (blue);

	}
}
