#pragma once

#include <SDL.h>
#include <vector>
#include "GameState.h"
#include "Renderer.h"
#include "EventHandle.h"

class Settings
{

};

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

	// State management.
	bool HasActiveState();
	GameState* State() { return states.back(); }
	void PushNewState(GameState* state);
	void PopLastState();

	Renderer GetRenderer() { return renderer; }

	Settings settings;

	int menuOption = 0;
	bool running = false;

private:	
	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;