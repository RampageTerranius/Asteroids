#include "Renderer.h"
#include "Debug.h"

bool Renderer::Init(const char* name, int width, int height, bool fullScreen, bool VSync)
{
	Uint32 renderFlags;
	Uint32 windowFlags;

	if (VSync)
		renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	else
		renderFlags = SDL_RENDERER_ACCELERATED;

	if (fullScreen)
		windowFlags = SDL_WINDOW_FULLSCREEN;
	else
		windowFlags = SDL_WINDOW_OPENGL;

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
	this->renderer = SDL_CreateRenderer(this->window, -1, renderFlags);

	if (this->window == nullptr)
	{
		debug.Log("Renderer", "CreateWindow", "Failed to create SDL Window");
		return false;
	}

	if (this->renderer == nullptr)
	{
		debug.Log("Renderer", "CreateWindow", "Failed to create SDL Renderer");
		return false;
	}

	return true;
}