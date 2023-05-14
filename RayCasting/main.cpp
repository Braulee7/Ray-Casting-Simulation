#include "App.hpp"

int main(int argc, char* argv[]) {
	App Raycaster(720, 1280, argc, argv);

	Raycaster.Run();

	return 0;
}