#include <memory>
#include <iostream>
#include <stdlib.h>
#include "Application.h"

App::App()
	:scene()
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

		//create the scene
		glm::vec3 origin = glm::vec3(0, 0, -1);
		glm::vec3 color = glm::vec3(0, 0, 224);
		scene.add(std::make_shared<Sphere>(origin, 0.5f, color, 0.0f));


		origin.x = -2; origin.y = 0; origin.z = 0;
		color.x = 225; color.y = 0; color.z = 0;
		scene.add(std::make_shared<Sphere>(origin, 0.5f, color, 0.1f, 1.0f));
		
		origin.x = -1; origin.y = 0; origin.z = 0;
		color.x = 1; color.y = 1; color.z = 1;
		scene.add(std::make_shared<Sphere>(origin, 0.5f, color, 0.1f, 1.0f));

		origin.y = -100.5;
		color.x = 0; color.y = 255; color.z = 0;
		scene.add(std::make_shared<Sphere>(origin, 100, color, 0.2f, 0.0f));

		mImg.Init(1280, 720, mRenderer, scene, mCam);

		



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

	
	Uint64 curr = SDL_GetPerformanceCounter();
	Uint64 last = 0;
	double delta = 0;

	while (mRunning) {
		last = curr;
		curr = SDL_GetPerformanceCounter();
		delta = (double)((curr - last) * 1000 / (double)SDL_GetPerformanceFrequency());
		while (SDL_PollEvent(&event) != 0) {
			Event(&event);
		}

		Loop();
		Render();
		
		system("cls");
		std::cerr << "Time since last render: " << delta << "ms" << std::endl;
		
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


	mImg.Render();

	SDL_RenderPresent(mRenderer);
}
