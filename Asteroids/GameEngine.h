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
	std::string SCREEN_NAME = "Asteroids";
	int SCREEN_WIDTH = 1024;
	int SCREEN_HEIGHT = 768;
	float BULLET_VELOCITY = 7.5;
	Uint32 FRAME_RATE = 60;

	float TURN_RATE = 2.0f;
	float VEL_INC = 0.05f;
	float MAX_VEL = 5.0f;
	int BULLET_DISTANCE = 90;

	bool AUTO_SPAWN_ASTEROIDS = true;
	float MAX_ASTEROID_VEL = 2.0f;
	int AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX = 200;
	int AUTO_SPAWN_ASTEROIDS_TIMER = 300;
	float AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER = 50;

private:	
	void LoadSettings();
	bool FULLSCREEN = false;
	bool VSYNC = false;

	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;