#include "BaseSDL.h"
#undef main

int SDL_main(int argc, char* argv[])
{
	MyApp SDLApp("Voronoi", 800, 600);
	SDLApp.Run();

	return 0;
}