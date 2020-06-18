#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>

#include "GameEngine.h"
#include "Debug.h"

void GameEngine::Init()
{
	if (this->running)
	{ 
		debug.Log("GameEngine", "Init", "Failed to initialize engine, Engine is already running");
		return;
	}

	debug.showMessagesOnConsole = true;

	debug.Log("GameEngine", "Init", "Starting setup...");

	debug.Log("GameEngine", "Init", "Initializing SDL sub-routines...");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string str = SDL_GetError();
		debug.Log("GameEngine", "Init", "SDL failed to initialize | " + str);
		return;
	}

	debug.Log("GameEngine", "Init", "Initializing SDL_TTF sub-routines...");

	if (TTF_Init() != 0)
	{
		std::string str = SDL_GetError();

		debug.Log("GameEngine", "Init", "TTF failed to initialize | " + str);
		return;
	}

	debug.Log("GameEngine", "Init", "Initializing SDL_MIXER sub-routines...");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::string str = Mix_GetError();

		debug.Log("GameEngine", "Init", "Mixer failed to initialize | " + str);
		return;
	}

	Mix_AllocateChannels(32);

	// Prepare the renderer
	this->renderer = Renderer();
	this->renderer.Init("Test", 800, 600, false, false);

	// Prepare the game states.
	GameState* mainMenu = new GameState_MainMenu();
	GameState* playField = new GameState_PlayField();

	this->stateList.push_back(mainMenu);
	this->stateList.push_back(playField);

	this->state = mainMenu;
	
	debug.Log("GameEngine", "Init", "Completed setup");
	this->running = true;
}

void GameEngine::Cleanup()
{
	debug.Log("GameEngine", "Cleanup", "Cleaning up engine...");

	debug.Log("GameEngine", "Cleanup", "Cleaning up states...");
	for (auto var : this->stateList)
		var->Cleanup();

	for (auto var : this->stateList)
		delete var;

	debug.Log("GameEngine", "Cleanup", "Cleaning up sub-routines...");
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	debug.Log("GameEngine", "Cleanup", "Cleanup complete");
}

void GameEngine::HandleInput()
{	
	this->running = state->HandleInput();
}

void GameEngine::HandleEvents()
{
	state->HandleEvents();
}

void GameEngine::Render()
{
	state->Render();
}

bool GameEngine::HasActiveState()
{
	if (this->state == nullptr)
		return false;
	else 
		return true;
}

GameEngine game = GameEngine();