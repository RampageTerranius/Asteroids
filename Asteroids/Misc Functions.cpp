#include "Misc Functions.h"

#include <Windows.h>
#include <PathCch.h>
#include <SDL.h>

// Gets the full path of the EXE file (minus the exe name).
std::string GetEXEPath()
{
	char* exePath;

	exePath = SDL_GetBasePath();
	std::string str = exePath;
	SDL_free(exePath);

	return exePath;
}

float GetDistance(float x1, float y1, float x2, float y2)
{
	return static_cast <float> (sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) ));
}