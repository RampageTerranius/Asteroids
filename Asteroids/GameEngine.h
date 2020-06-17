#pragma once

#include <SDL.h>
#include "GameState.h"
#include "Renderer.h"
#include "EventHandle.h"

class GameEngine
{
public:
	// Startup and shutdown of engine.
	void Init();
	void Cleanup();

	// Loop management.
	void HandleInput();
	//void HandleEvents();
	//void Render();

	bool Running() { return running; }
	void Shutdown() { running = false; }

	// State management.

private:
	bool running = false;
	Renderer renderer = Renderer();
};

extern GameEngine game;