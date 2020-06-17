#pragma once

#include <SDL.h>

class Renderer
{
public:
	bool Init(const char* name, int width, int height, bool fullScreen, bool VSync);

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};