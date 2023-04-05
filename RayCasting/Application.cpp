#include "Application.h"

App::App()
{
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

	mWindow = SDL_CreateWindow("Braulee RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);

	if (mWindow) {
		//successfully created
		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

		mImg.Init(1000, 1000, mRenderer);

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
		while (SDL_PollEvent(&event) != 0) {
			Event(&event);
		}

		Loop();
		Render();
	}

	return 1;
}

void App::Loop()
{
}

void App::Event(SDL_Event* event)
{
	if (event->type == SDL_QUIT)
		mRunning = false;
}

void App::Render()
{
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	mImg.Render();

	SDL_RenderPresent(mRenderer);
}
