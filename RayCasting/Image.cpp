#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include <iostream>
#include <cmath>
#include <limits>



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

void Image::Render()
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
			mImageData[x + y * mWidth] = fragShader(coord);
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
	SDL_RenderCopy(mRenderer, mTexture, &src, &bound);
	
}

//meant return a color for each pixel on the screen
uint32_t Image::fragShader(glm::vec2 coord)
{
	//shoot a ray from the screen torward the pixel
	Ray ray(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(coord.x, coord.y, -1.0f));

	//sphere object we're going to render
	Sphere sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);

	hitRecord rec;

	if (sphere.hit(ray, 0, std::numeric_limits<float>::infinity(), rec))
		return 0xffff00ff;
	else
		return 0xff000000;
}
