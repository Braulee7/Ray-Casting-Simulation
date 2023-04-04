#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <vector>
#include <string>
#include <glm.hpp>

class Image {
public:
	Image();
	~Image();

	void Init(const int x, const int y, SDL_Renderer* renderer);
	void Render();
	void InitTexture();


private:
	//height and width of the image
	int mWidth, mHeight;
	//the data of the image
	uint32_t *mImageData;
	//SDL2 rendering stuff to see in real time
	//the image
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;

	//gpu rendering TODO
	uint32_t fragShader(glm::vec2 coord);

};

#endif
