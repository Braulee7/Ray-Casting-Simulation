#ifndef APP_HPP
#define APP_HPP

#include <SDL.h>
#include <iostream>
#include <vector>
#include <execution>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include "Scene.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "Sphere.hpp"
#include "Mesh.hpp"

class App {
public:
	App(int height, int width, int argc, char* argv[]) {
		//dimensions of image
		mWidth = width;
		mHeight = height;
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


		//parse the command line
		CommandLine(argc, argv);
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

		

		//application loop
		while (mRunning) {
			//check inputs from user
			while (SDL_PollEvent(&e) != 0)
				Event(&e);
			//get raycasted image
			Render();
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

		mWindow = SDL_CreateWindow("Braulee RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		
		if (!mWindow) return -2; //failure to create window

		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
		InitTexture();
	
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

	void CommandLine(int argc, char* argv[]) {
		if (argc < 2)
			//load default image
			return;

		std::string file;
		//parse command lines
		for (int i = 1; i < argc; i++) {
			/* Optional file to load in a scene */
			if (std::string(argv[i]) == "--scene" || std::string(argv[i]) == "-s") {
				if (i + 1 > argc) {
					std::cerr << "No source file selected, loading default scene...\n";
					return;
				}
				file = argv[++i];
			}

			/* help to know command line arguments*/
			 else if (std::string(argv[i]) == "--help" || std::string(argv[i]) == "-h") {
				std::cerr << "Optional Arguments:\n"
					<< "--scene / -s: load in a scene from a .braulee file\n"
					<< "\tArguments: filepath to the scene file\n"
					<< "--help / -h: bring up list of commands\n"
					<< "--DMT: allows multithreading to be used in the rendering, speeds up processes\n";
				exit(0);	//help automatically exits
			}
			 else if (std::string(argv[i]) == "--DMT") {
					#define MT 1
			}

			/* unkown command*/
			 else {
				std::cerr << "Uknown command: " << argv[i] << " failure to process\n";
				exit(-1);
			}
		}
		
		//parse the file to get scene
		LoadScene(file);
	}

	void LoadScene(std::string& filePath) {
		if (!std::filesystem::exists(filePath)) {
			std::cerr << "Failed to find scene, check your filepath and ensure it's correct.\n";
			exit(-1);
		}

		std::ifstream in(filePath, std::ios::in);
		if (!in)
		{
			std::cerr << "Cannot open " << filePath << std::endl;
			exit(1);

		}
		//mesh to hold our triangles
		Mesh tempMesh;

		//points to hold the three vertices of a triangle
		glm::vec3 points[3];
		glm::vec3 col(255);
		std::string line;

		int count = 0;
		while (std::getline(in, line))
		{
			//check v for vertices
			if (line.substr(0, 2) == "v ") {
				std::istringstream v(line.substr(2));
				float x, y, z;
				v >> x; v >> y; v >> z;
				points[count++] = glm::vec3(x * 2, y * 2 , z * -2);

				if (count > 2) {
					//reset the count
					count = 0;
					//add triangle to the mesh
					tempMesh.Add(points[0], points[1], points[2], col);
				}

			}

		}

		//add the mesh to the scene
		mScene.add(std::make_shared<Mesh>(tempMesh));

	}

	void LoadDefault() {
		glm::vec3 origin, col;
		
		origin.x = 0; origin.y = 0; origin.z = -2;
		col.x = 255; col.y = 0; col.z = 255;
		mScene.add(std::make_shared<Sphere>(origin, 5.0f, col));
		
		origin.x = -2; origin.y = 0; origin.z = -2;
		col.x = 1; col.y = 1; col.z = 5;
		mScene.add(std::make_shared<Sphere>(origin, 5.0f, col, 1.0f));
		
		origin.x = 2; origin.y = 0; origin.z = -2;
		col.x = 1; col.y = 1; col.z = 1;
		mScene.add(std::make_shared<Sphere>(origin, 5.0f, col, 1.0f));
	}

	void Event(SDL_Event* e) {
		
		switch (e->type) {
		case SDL_QUIT:
			mRunning = false;
			break;

		//if user resized the window 
		case SDL_WINDOWEVENT:
			if (e->window.event == SDL_WINDOWEVENT_RESIZED)
				Resize(e->window.data1, e->window.data2);
		}
	}

	void Resize(uint32_t width, uint32_t height) {
		//reget the height and width of image
		mWidth = width;
		mHeight = height;
		mWidthIt.resize(mWidth);
		mHeightIt.resize(mHeight);

		mImage.Resize(mWidth, mHeight);

		//recast to get the new location of the image
		mImage.DeleteImage();
	}

	void Render() {
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);

		//if the image wasn't already rendered
		//static rendering so no need to repeat
		//math if not needed
		if (!mImage.mImageData) {
			// Perfomance variables to check render time
			Uint64 start = SDL_GetPerformanceCounter();

			RenderRayCast();

			Uint64 end = SDL_GetPerformanceCounter();

			std::cerr << "Image took " << (float)((end - start) / SDL_GetPerformanceFrequency()) << "s to render\n";
		}

		SDL_UpdateTexture(mTexture, nullptr, mImage.mImageData, mWidth * sizeof(Uint32));


		SDL_Rect src, bound;
		src.x = 0;
		src.y = 0;
		src.w = mWidth;
		src.h = mHeight;
		bound = src;
		SDL_RenderCopyEx(mRenderer, mTexture, &src, &bound, 0, NULL, SDL_FLIP_VERTICAL);

		SDL_RenderPresent(mRenderer);
	}

	//TODO loading image if needed
	//or a scene set up
	void RenderDefault() {
		
	}

	//math operations to get the 
	//raycasting working
	void RenderRayCast() {

		//render the scene
		mImage.SetImage();
		int pSamples = 10;
#if MT
		/* MULTITHREADED path if user enables it
		*  Eats up all processing units so can't
		* have anything else running
		*/
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
		/* Single unparalleled path if user does not enable */
		for (uint32_t y = 0; y < mHeight; y++) {
			for (uint32_t x = 0; x < mWidth; x++) {
				int index = x + y * mWidth;
				accumulatedColor[index] = glm::vec3(0);
				for (int i = 0; i < pSamples; i++) {
					col = Pixel(x, y);	
					accumulatedColor[index] += col;
				}
				accumulatedColor[index] = glm::clamp(accumulatedColor[index], glm::vec3(0), glm::vec3(255));

				mImage.mImageData[index] = BU::Color(accumulatedColor[index], pSamples);
			}
		}
#endif

	}

	// Function to get each individual pixel color
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
				ray.mDirection = glm::normalize(dir);
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
