#pragma once

#include <SDL.h>
#include <vector>
#include "GameState.h"
#include "Renderer.h"
#include "EventHandle.h"

class GameEngine
{
public:
	GameEngine();
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

	// Global renderer for engine.
	Renderer GetRenderer() { return renderer; }
	
	bool running = false;

	int fps;

	// Game related variables.

	const int SCREEN_WIDTH = 1024;
	const int SCREEN_HEIGHT = 768;
	const float BULLET_VELOCITY = 7.5;
	const Uint32 FRAME_RATE = 60;
	const int FRAME_TIME = 1000 / FRAME_RATE;

	const float TURN_RATE = 2.0f;
	const float VEL_INC = 0.05f;
	const float MAX_VEL = 5.0f;
	const int BULLET_DISTANCE = 90;

	const bool AUTO_SPAWN_ASTEROIDS = true;
	const float MAX_ASTEROID_VEL = 2.0f;
	const int AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX = 200;
	const int AUTO_SPAWN_ASTEROIDS_TIMER = 300;
	const float AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER = 50;

private:	
	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;