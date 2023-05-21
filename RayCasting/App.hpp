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
#include <cstring>
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

private://variables

	//SDL Variables
	bool mRunning;
	SDL_Texture* mTexture;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	//rendering variables
	Camera mCam;
	Image mImage;
	std::vector<uint32_t> mWidthIt, mHeightIt;
	glm::vec3 col;
	glm::vec3* accumulatedColor;
	Scene mScene;
	uint32_t mWidth, mHeight;

	//check if user wants to multithread
	bool multiThreading = false;



private://functions


	/* Initialises SDL variables to create the image
	*  and present it to the user
	*/
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

		// check the endianess of machine to get 
		// correct bit order
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
		if (argc < 2) return; // no commands to parse 

		std::string file = "";
		//parse command line
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
				multiThreading = true;
			}

			/* unkown command*/
			 else {
				std::cerr << "Uknown command: " << argv[i] << " failure to process\n";
				exit(-1);
			}
		}

		if (file == "")
			LoadDefault();
		else
			//parse the file to get scene
			LoadScene(file);
	}

	/* Loads in an .obj file from the user if necessary
	*  Has to make sure that the file is created with 
	*  Triangles in mind
	*/
	void LoadScene(std::string& filePath) {
		if (!std::filesystem::exists(filePath)) {
			std::cerr << "Failed to find scene, check your filepath and ensure it's correct.\n";
			exit(-1);
		}

		//to parse file
		std::stringstream ss;
		std::ifstream fin(filePath);
		std::string line = "";
		std::string prefix = "";
		glm::vec3 tempVec3;
		int tempIndex = 0;

		//to hold the actual data
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<int> posIndices;
		std::vector<int> normIndices;

		/* error check the file*/
		if (!fin.is_open()) {
			std::cerr << "Failed to open the file check path.\n";
			exit(-1);
		}

		/* Parse the file to get necessary information to render the image*/
		while (std::getline(fin, line)) {
			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "v") {	//vertex
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				vertices.push_back(tempVec3);

			}
			else if (prefix == "vn") {	//normals
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				normals.push_back(tempVec3);
			
			}

			else if (prefix == "f") {	//face
				int count = 0;
				while (ss >> tempIndex) {
					switch (count) {
					case 0: //vertex position
						
						posIndices.push_back(tempIndex - 1);
						break;
					case 2:	//vertex normals
						
						normIndices.push_back(tempIndex - 1);
					}

					if (ss.peek() == '/') {
						++count;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ') {
						count = 0;
						ss.ignore(1, ' ');
					}
				}
			}

		}

		//index buffer to build the object
		BuildObj(vertices, normals, posIndices, normIndices);
	}

	/* If there is no scene loaded in render a default image
	*/
	void LoadDefault() {

		glm::vec3 origin, col;
		
		origin.x = 0; origin.y = 0; origin.z = -2;
		col.x = 255; col.y = 0; col.z = 255;
		mScene.add(std::make_shared<Sphere>(origin, .5f, col));
		
		origin.x = -2; origin.y = 0; origin.z = -2;
		col.x = 1; col.y = 1; col.z = 5;
		mScene.add(std::make_shared<Sphere>(origin, .5f, col, 1.0f));
		
		origin.x = 2; origin.y = 0; origin.z = -2;
		col.x = 1; col.y = 1; col.z = 1;
		mScene.add(std::make_shared<Sphere>(origin, .5f, col, 1.0f));


		glm::vec3 pointA(-1, 0, -1), pointB(0, 1, -1), pointC(1, 0, -1);
		col.x = 255; col.y = 0; col.z = 255;
		mScene.add(std::make_shared<Triangle>(pointA, pointB, pointC, col));

	}

	void BuildObj(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<int> position_index, std::vector<int> normal_index) {

		Mesh mesh;

		int count = 0;	//count to make sure we add a triangle every 3 indices

		//		  position		normals			color
		glm::vec3 triangle[3] = { glm::vec3(0) }, normal[3] = { glm::vec3(0) }, col(245, 245, 220);

		//use the indices to add create the mesh
		for (int i = 0; i < position_index.size(); i++) {
			triangle[count] = vertices[position_index[i]];
			normals[count++] = normals[normal_index[i]];

			if (count > 2) {
				count = 0;
				mesh.Add(triangle[0], triangle[1], triangle[2], normal[0], normal[1], normal[2], col);
			}
		}

		//add random spheres around to scene
		glm::vec3 origin, color;
		origin.x = 0; origin.y = -100; origin.z = -10;
		color.r = 0; color.g = 255; color.b = 0;
		mScene.add(std::make_shared<Sphere>(origin, 100.0f, color, 0.0f));

		origin.x = -2; origin.y = 1; origin.z = -1;
		color.r = 255; color.g = color.b = 1;
		mScene.add(std::make_shared<Sphere>(origin, 0.5f, color, 1.0f));
		
		origin.x = 2; origin.y = 1; origin.z = -3;
		color.r = 0; color.g = 0; color.b = 255;
		mScene.add(std::make_shared<Sphere>(origin, 0.5f, color, 1.0f));


		origin.y = 0; origin.z = -2;
		color.r = 255; color.b = 255;
		mScene.add(std::make_shared<Sphere>(origin, 0.55, color, 0.75f));
		
		origin.x = 5;
		color.r = 233; color.g = 122; color.b = 54;
		mScene.add(std::make_shared<Sphere>(origin, 0.55, color, 0.75f));
		
		origin.x = -2; origin.z = -5;
		color.r = 23; color.g = 23; color.b = 111;
		mScene.add(std::make_shared<Sphere>(origin, 0.55, color, 0.75f));
		
		origin.x = -4; origin.z = -2;
		color.r = 23; color.g = 5; color.b = 76;
		mScene.add(std::make_shared<Sphere>(origin, 0.55, color, 0.75f));
		
		origin.x = -3; origin.z = -2;
		color.r = 2; color.g = 233; color.b = 200;
		mScene.add(std::make_shared<Sphere>(origin, 0.55, color, 0.25f));

		mScene.add(std::make_shared<Mesh>(mesh));
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

		/* Present the image to SDL and user*/
		SDL_Rect src, bound;
		src.x = 0;
		src.y = 0;
		src.w = mWidth;
		src.h = mHeight;
		bound = src;
		SDL_RenderCopyEx(mRenderer, mTexture, &src, &bound, 0, NULL, SDL_FLIP_VERTICAL);

		SDL_RenderPresent(mRenderer);
	}

	//math operations to get the 
	//raycasting working
	void RenderRayCast() {

		//render the scene
		mImage.SetImage();
		int pSamples = 10;
		if (multiThreading) {
			/* MULTITHREADED path if user enables it
			*  Eats up all processing units so can't
			* have anything else running
			*/
			std::for_each(std::execution::par, mHeightIt.begin(), mHeightIt.end(),
				[this, pSamples](uint32_t y) {
					std::for_each(std::execution::par, mWidthIt.begin(), mWidthIt.end(),
					[this, y, pSamples](uint32_t x) {
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
		}
		else {
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
		}

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

				//calculate color color of hit object with the intensity of the light
				float intensity = glm::max(glm::dot(normal, -mScene.mLightDirection), 0.1f);
				glm::vec3 sphereCol = mat.mColor;
				sphereCol *= intensity;
				sphereCol = BU::normalize(sphereCol);
				color += sphereCol * multiplier;
				multiplier *= 0.5f;

				//get direction of next ray based on smoothness of object
				glm::vec3 spec = BU::reflect(ray.mDirection, normal);
				glm::vec3 diff = BU::Diffuse(normal);
				glm::vec3 dir = BU::lerp(diff, spec, mat.mSmoothness);

				//redirect ray
				ray.mOrigin = rec.hitPoint + normal * 1e-5f;
				ray.mDirection = glm::normalize(dir);
			}
			else {
				//missed so generate sky color
				glm::vec3 sky(0);
				color += sky * multiplier;
				break;
			}
		}

		return color;
	}

};
#endif // !APP_HPP
