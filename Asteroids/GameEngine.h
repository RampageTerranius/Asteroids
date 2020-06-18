#pragma once

#include <SDL.h>
#include <vector>
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
	void HandleEvents();
	void Render();

	bool Running() { return running; }
	void Shutdown() { running = false; }

	// State management.
	bool HasActiveState();

	Renderer GetRenderer() { return renderer; }

private:
	bool running = false;
	Renderer renderer = Renderer();
	GameState* state = nullptr;
	std::vector<GameState*> stateList;
};

extern GameEngine game;