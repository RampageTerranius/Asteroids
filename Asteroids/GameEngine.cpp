#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>

#include "GameEngine.h"
#include "MainMenu.h"
#include "PlayField.h"
#include "Misc Functions.h"
#include "Debug.h"

GameEngine::GameEngine()
{	
	Init();	
}

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
	this->renderer.Init("Test", this->SCREEN_WIDTH, this->SCREEN_HEIGHT, false, false);

	// Prepare the game state.
	this->PushNewState(new GameState_MainMenu());
	
	debug.Log("GameEngine", "Init", "Completed setup");
	this->running = true;	
}

void GameEngine::Cleanup()
{
	debug.Log("GameEngine", "Cleanup", "Cleaning up engine...");

	debug.Log("GameEngine", "Cleanup", "Cleaning up states...");
	for (auto var : this->states)
		var->Cleanup();

	for (auto var : this->states)
		delete var;

	debug.Log("GameEngine", "Cleanup", "Cleaning up sub-routines...");
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

	debug.Log("GameEngine", "Cleanup", "Cleanup complete");
}

// Ask the current GameState to run its HandleInput code.
void GameEngine::HandleInput()
{	
	this->running = this->State()->HandleInput();
}

// Ask the current GameState to run its HandleEvents code.
void GameEngine::HandleEvents()
{
	this->State()->HandleEvents();
}

// Ask the current GameState to run its Render code.
void GameEngine::Render()
{
	this->State()->Render();
}

void GameEngine::PushNewState(GameState* state)
{
	states.push_back(state);
	debug.Log("GameEngine", "PushNewState", "Pushed new state as active.");
}

void GameEngine::PopLastState()
{
	if (this->states.size() > 0)
	{
		this->states.back()->Cleanup();
		delete this->states.at(this->states.size() - 1);
		this->states.pop_back();
		debug.Log("GameEngine", "PopLastState", "Revereted to previous state.");
	}
}

bool GameEngine::HasActiveState()
{
	if (this->State() == nullptr)
		return false;
	else 
		return true;
}

GameEngine game = GameEngine();