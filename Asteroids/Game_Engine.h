#pragma once
#include <vector>

#include "GameState.h"
#include "Renderer.h"

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
	Uint32 FRAME_RATE = 60;

	double TURN_RATE = 2.0;
	double VEL_INC = 0.05;
	double MAX_VEL = 5.0;
	int BULLET_DISTANCE = 90;
	double BULLET_VELOCITY = 7.5;

	bool AUTO_SPAWN_ASTEROIDS = true;
	double MAX_ASTEROID_VEL = 2.0f;
	int AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX = 200;
	int AUTO_SPAWN_ASTEROIDS_TIMER = 300;
	double AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER = 50;

	class Controls
	{
	public:
		int fire = SDLK_SPACE;
		int forwards = SDLK_w;
		int backwards = SDLK_s;
		int rotateLeft = SDLK_a;
		int rotateRight = SDLK_d;
		int boost = SDLK_LSHIFT;
		int equalize = SDLK_c;
		int createAsteroid = SDLK_f;
	}controls;

private:	
	void LoadSettings();
	bool FULLSCREEN = false;
	bool VSYNC = false;

	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;