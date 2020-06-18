#pragma once

#include <SDL.h>

class Renderer
{
public:
	bool Init(const char* name, int width, int height, bool fullScreen, bool VSync);

	SDL_Window* window;
	SDL_Renderer* renderer;

	int WindowWidth() { return windowWidth; }
	int WindowHeight() { return windowHeight; }

private:
	int windowWidth;
	int windowHeight;
};