#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL.h>
#include "Image.h"

class App {
public:
	App();
	~App();
	int Run();
	

private:
	Image mImg;
	bool mRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	
private:
	int Init();
	void Loop();
	void Event(SDL_Event* event);
	void Render();

};

#endif // !APPLICATION_H
