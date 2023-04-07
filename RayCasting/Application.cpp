#include <memory>
#include <iostream>
#include <stdlib.h>
#include "Application.h"

App::App()
{
	glm::vec3 pos = glm::vec3(0, 0, 2);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	mCam = Camera(pos, front, up, 90, (float)16 / 9);
	mRunning = true;
	mWindow = nullptr;
	mRenderer = nullptr;
}

App::~App()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;
	SDL_Quit();
}

int App::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 0;
	}

	mWindow = SDL_CreateWindow("Braulee RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

	if (mWindow) {
		//successfully created
		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

		mImg.Init(1280, 720, mRenderer);

	}
	else {
		return 0;
	}

	return 1;
}

int App::Run()
{
	SDL_Event event;

	if (!Init())
		return 0;

	

	while (mRunning) {
		Uint32 start = SDL_GetPerformanceCounter();
		
		while (SDL_PollEvent(&event) != 0) {
			Event(&event);
		}

		Loop();
		Render();

		Uint32 end = SDL_GetPerformanceCounter();

		float fps = (end - start) / (float) SDL_GetPerformanceFrequency();
		
		system("cls");
		std::cerr << "FPS: " << 1.0f / fps << std::fflush;
		
	}

	return 1;
}

void App::Loop()
{
}

void App::Event(SDL_Event* event)
{

	switch (event->type) {
	
	case SDL_QUIT:
		mRunning = false;
		break;
	}

}

void App::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	glm::vec3 origin = glm::vec3(0, 0, -1);
	glm::vec3 color = glm::vec3(200, 0, 200);
	HittableList scene(std::make_shared<Sphere>(origin, 0.5, color));
	
	origin.y = -100.5;
	color.x = 0; color.y = 255; color.z = 0;
	scene.add(std::make_shared<Sphere>(origin, 100, color));

	origin.x = -2; origin.y = 0; origin.z = 0;
	color.x = 125; color.y = 125; color.z = 32;
	scene.add(std::make_shared<Sphere>(origin, 0.5, color));


	mImg.Render(scene, mCam);

	SDL_RenderPresent(mRenderer);
}
