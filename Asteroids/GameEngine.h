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

	// State management.
	bool HasActiveState();
	GameState* State() { return states.back(); }
	void PushNewState(GameState* state);
	void PopLastState();

	Renderer GetRenderer() { return renderer; }
	
	bool running = false;

	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const float TURN_RATE = 0.1f;
	const float VEL_INC = 0.001f;
	const float MAX_VEL = 0.1f;

private:	
	Renderer renderer = Renderer();
	std::vector<GameState*> states;

	
};

extern GameEngine game;