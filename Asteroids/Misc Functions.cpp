#include "Misc Functions.h"

#include <Windows.h>
#include <PathCch.h>
#include <SDL.h>

// Gets the full path of the EXE file (minus the exe name).
std::string GetEXEPath()
{
	char exePath[MAX_PATH];

	GetModuleFileNameA(NULL, exePath, sizeof(exePath));

	std::string tempStr = exePath;

	size_t pos = tempStr.find_last_of("\\");

	if (pos != std::string::npos)
		return tempStr.substr(0, pos + 1);

	return "";
}

float GetDistance(SDL_Point pointA, SDL_Point pointB)
{
	return sqrt( pow(pointB.x - pointA.x, 2) + pow(pointB.y - pointA.y, 2) );
}

float RandomFloat(float min, float max)
{
	return ((static_cast<float> (rand()) / static_cast<float> (RAND_MAX)) * (max - min)) + min;
}