#pragma once
#include <SDL.h>

class Renderer
{
public:
	bool Init(const char* name, int width, int height, bool fullScreen, bool VSync);

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int WindowWidth() { return windowWidth; }
	int WindowHeight() { return windowHeight; }

	SDL_Color renderColor = {0, 0, 0, 255};

private:
	int windowWidth = 640;
	int windowHeight = 480;
};