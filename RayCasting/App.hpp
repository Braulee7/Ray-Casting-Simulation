#ifndef APP_HPP
#define APP_HPP

#include <SDL.h>
#include <iostream>
#include <vector>
#include <execution>
#include <fstream>
#include <filesystem>
#include "Scene.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "Sphere.hpp"


class App {
public:
	App(int height, int width) {
		//dimensions of image
		mWidth = width;
		mHeight = height;
		std::cout << mWidth << '\t' << mHeight << std::endl;
		mImage = Image(width, height);

		//render variables
		col = glm::vec3(0);
		accumulatedColor = new glm::vec3[mWidth * mHeight];
		mWidthIt.resize((uint32_t)mWidth);
		mHeightIt.resize((uint32_t)mHeight);

		for (int i = 0; i < mWidth; i++)
			mWidthIt[i] = i;
		for (int i = 0; i < mHeight; i++)
			mHeightIt[i] = i;

		//create the camera
		glm::vec3 pos = glm::vec3(0, 0, 2);
		glm::vec3 front = glm::vec3(0, 0, -1);
		glm::vec3 up = glm::vec3(0, 1, 0);
		mCam = Camera(pos, front, up, 90, (float)width / height);

		//scene initialisation
		mScene = Scene();

		//SDL variables initialised later
		mRunning = true;
		mWindow = nullptr;
		mRenderer = nullptr;
		mTexture = nullptr;
	}
	~App() {
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mWindow);
		SDL_Quit();
		delete[] accumulatedColor;
	}
	int inline Run() {
		SDL_Event e;

		if (Init() < 0) return 0;

		// Perfomance variables to check render time
		Uint64 curr = SDL_GetPerformanceCounter();
		Uint64 last = 0;
		double delta = 0;

		//application loop
		while (mRunning) {

			//get render time from last frame
			last = curr;
			curr = SDL_GetPerformanceCounter();
			delta = (double)((curr - last) * 1000 / (double)SDL_GetPerformanceFrequency());

			//check inputs from user
			while (SDL_PollEvent(&e) != 0)
				Event(&e);

			//render the scene
			Render();

			//show render time to user
			system("cls");
			std::cerr << "Time since last render: " << delta << "ms\n";
		}

		return 1;
	}

private:
	bool mRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Camera mCam;
	Image mImage;
	SDL_Texture* mTexture;
	std::vector<uint32_t> mWidthIt, mHeightIt;
	
	
	glm::vec3 col;
	glm::vec3* accumulatedColor;
	Scene mScene;

	uint32_t mWidth, mHeight;



private:
	int Init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return -1;

		

		mWindow = SDL_CreateWindow("Braulee RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN);
		
		if (!mWindow) return -2; //failure to create window

		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
		InitTexture();
		//create the scene
		glm::vec3 ori(0, 0, -1);
		glm::vec3 color(1, 1, 1);

		mScene.add(std::make_shared<Sphere>(ori, 0.5f, color, 1));

		ori.x = -2; 
		color.r = 255; color.b = 0;
		mScene.add(std::make_shared<Sphere>(ori, 0.5f, color, 1.0f));
		
		ori.x = 0; ori.y = -100; ori.z = -10;
		color.r = 0; color.g = 255;
		mScene.add(std::make_shared<Sphere>(ori, 100.0f, color));


		return 1; //success
	
	}

	void InitTexture() {
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
	void Event(SDL_Event* e) {
		
		switch (e->type) {
		case SDL_QUIT:
			mRunning = false;
			break;
		}
	}
	void Render() {
	
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		//render the scene
		mImage.SetImage();
#define MT 1

		int pSamples = 10;
#if MT
		std::for_each(std::execution::par, mHeightIt.begin(), mHeightIt.end(),
			[this, pSamples](uint32_t y) {
				std::for_each(std::execution::par, mWidthIt.begin(), mWidthIt.end(),
				[this , y, pSamples](uint32_t x) {
					int index = x + y * mWidth;
					accumulatedColor[index] = glm::vec3(0);
					for (int i = 0; i < pSamples; i++) {
						col = Pixel(x, y);
						accumulatedColor[index] += col;
					}
					accumulatedColor[index] = glm::clamp(accumulatedColor[index], glm::vec3(0), glm::vec3(255));

					mImage.mImageData[index] = BU::Color(accumulatedColor[index], pSamples);
					
				});
			});
#else
		for (uint32_t y = 0; y < mHeight; y++) {
			for (uint32_t x = 0; x < mWidth; x++) {
				int index = x + y * mWidth;

				for (int i = 0; i < pSamples; i++) {
					col += Pixel(x, y);	
				}
				mImage.mImageData[index] = BU::Color(col, pSamples);
				if (open)
					fout << index << "\n";
			}
		}
#endif

		SDL_UpdateTexture(mTexture, nullptr, mImage.mImageData, mWidth * sizeof(Uint32));

		mImage.DeleteImage();
		SDL_Rect src, bound;
		src.x = 0;
		src.y = 0;
		src.w = mWidth;
		src.h = mHeight;
		bound = src;
		SDL_RenderCopyEx(mRenderer, mTexture, &src, &bound, 0, NULL, SDL_FLIP_VERTICAL);

		SDL_RenderPresent(mRenderer);
	}

	glm::vec3 Pixel(uint32_t x, uint32_t y) {
		//get coord of pixel
		glm::vec2 coord = { (float)x / (float)mWidth, (float)y / (float)mHeight };
		coord = coord * 2.0f - 1.0f;

		//generate ray
		Ray ray = mCam.ray(coord.x, coord.y);

		hitInfo rec;
		glm::vec3 color(0);
		float multiplier = 1.0f;
		constexpr float inf = std::numeric_limits<float>::infinity();
		int bounces = 8;

		for (int i = 0; i < bounces; i++) {
			if (mScene.hit(ray, 0.00001, inf, rec)) {

				
				Material mat = rec.mat;
				glm::vec3 normal = rec.normal;

				float intensity = glm::max(glm::dot(normal, -mScene.mLightDirection), 0.0f);
				glm::vec3 sphereCol = mat.mColor;
				sphereCol *= intensity;
				sphereCol = BU::normalize(sphereCol);
				color += sphereCol * multiplier;
				multiplier *= 0.5f;

				glm::vec3 spec = BU::reflect(ray.mDirection, normal);
				glm::vec3 diff = BU::Diffuse(normal);
				glm::vec3 dir = BU::lerp(diff, spec, mat.mSmoothness);

				ray.mOrigin = rec.hitPoint + normal * 1e-5f;
				ray.mDirection = dir;
			}
			else {
				float t = 0.5 * ray.mDirection.y + 1;
				glm::vec3 sky = (1 - t) * glm::vec3(1) + t * glm::vec3(0.5, .7f, 1);
				sky = glm::vec3(0.6f, 0.7f, 0.9f);
				color += sky * multiplier;
				break;
			}
		}

		return color;
	}

};
#endif // !APP_HPP
