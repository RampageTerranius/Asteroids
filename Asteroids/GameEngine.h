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
	GameState* State() { return states.back(); }
	void PushNewState(GameState* state) { states.push_back(state); }
	void PopLastState() { states.pop_back(); }

	Renderer GetRenderer() { return renderer; }

private:
	bool running = false;
	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;