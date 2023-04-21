#include <iostream>
#include <cmath>
#include <limits>
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Util.h"


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
	int pSamples = 1;
	glm::vec3 col(0);

	//for each pixel of the image send it to our "frag shader"
	for (uint32_t y = 0; y < mHeight; y++) {
		for (uint32_t x = 0; x < mWidth; x++) {
			//get pixel averages
			for (int i = 0; i < pSamples; i++)
			{
				//get coordinate of individual pixel
				glm::vec2 coord = { (float)x / (float)mWidth, (float)y / (float)mHeight };
				coord = coord * 2.0f - 1.0f;

				Ray ray = cam.ray(coord.x, coord.y);

				col = fragShader(ray, scene);
			}

			//put the pixel average into the image
			mImageData[x + y * mWidth] = BU::Color(col, pSamples);
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
glm::vec3 Image::fragShader(Ray &ray, HittableList& scene)
{
	//shoot a ray from the screen torward the pixel
	//Ray ray(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(coord.x, coord.y, -1.0f));

	hitRecord rec;
	glm::vec3 col = glm::vec3(1);

	for (int i = 0; i < 50; i++)
	{
		if (scene.hit(ray, 0.00001, std::numeric_limits<float>::infinity(), rec)) {
			Material mat = rec.mat;
			col *= mat.colorVec();

			//reshoot ray from point of intersection (diffuse light)
			srand((unsigned int)(ray.getOrigin().x + ray.getOrigin().y));
			glm::vec3 target = rec.p + rec.normal + glm::vec3((float)(rand() % (RAND_MAX + 1)), (float)(rand() % (RAND_MAX + 1)),(float)(rand() % (RAND_MAX + 1)));
			ray = Ray(rec.p, target - rec.p);
		}
		else {
			break;

			glm::vec3 dir = ray.getDirection();

			auto unitDir = (dir) / glm::length(dir);
			float t = 0.5 * (unitDir.y + 1);

			uint8_t red = (uint8_t)((1 - t) * (1 * 255.999f)) + ((t) * (0.5 * 255.999f));
			uint8_t green = (uint8_t)((1 - t) * (1 * 255.999f)) + ((t) * (0.7 * 255.999f));
			uint8_t blue = (uint8_t)((1 - t) * (1 * 255.999f)) + ((t) * (1.0 * 255.999f));

			glm::vec3 sorround = glm::vec3((float)red, (float)green, (float)blue);

			col *= sorround;
		}
	}

	return col;

}
