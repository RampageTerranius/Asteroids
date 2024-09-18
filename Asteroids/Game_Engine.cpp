#include "Game_Engine.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>// nlohmann's JSON libary https://github.com/nlohmann/json


#include "Debug.h"
#include "Misc_Functions.h"
#include "MainMenu.h"
#include "PlayField.h"

GameEngine::GameEngine()
{	
	Init();	
}

// Loads the settings for the program.
void GameEngine::LoadSettings()
{
	using json = nlohmann::json;
	// Load our json file into memory.
	std::ifstream ifs(GetEXEPath() + "Settings.json");

	// In the situation we fail to load the settings file return to defaults and create a new file.
	if (!ifs.is_open())
	{
		json configFile;

		// Create video settings.
		json video;
		video["ScreenName"] = SCREEN_NAME;
		video["ScreenWidth"] = SCREEN_WIDTH;
		video["ScreenHeight"] = SCREEN_HEIGHT;
		video["FrameRate"] = FRAME_RATE;
		video["FullScreen"] = false;
		video["vSync"] = false;
		
		// Create ship settings.
		json ship;
		ship["TurnRate"] = TURN_RATE;
		ship["VelocityIncreasePerTick"] = VEL_INC;
		ship["MaxVelocity"] = MAX_VEL;

		// Create bullet settings.
		json bullet;
		bullet["BulletDistance"] = BULLET_DISTANCE;
		bullet["BulletVelocity"] = BULLET_VELOCITY;

		// Create asteroid settings.
		json asteroid;
		asteroid["AutoSpawnAsteroids"] = true;
		asteroid["MaxAsteroidVelocity"] = MAX_ASTEROID_VEL;
		asteroid["AutoSpawnAsteroidsTotalSizeMax"] = AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX;
		asteroid["AutoSpawnAsteroidsTimer"] = AUTO_SPAWN_ASTEROIDS_TIMER;
		asteroid["AutoSpawnAsteroidsDistanceFromPlayer"] = AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER;

		// Create control settings.
		json controls;
		controls["Fire"] = SDLK_SPACE;
		controls["Forwards"] = SDLK_w;
		controls["Backwards"] = SDLK_s;
		controls["RotateLeft"] = SDLK_a;
		controls["RotateRight"] = SDLK_d;
		controls["Boost"] = SDLK_LSHIFT;
		controls["Equalize"] = SDLK_c;
		controls["CreateAsteroid"] = SDLK_f;

		// Assign each section of our json file to the config object.
		configFile["Controls"] = controls;
		configFile["Asteroid"] = asteroid;
		configFile["Bullet"] = bullet;
		configFile["Ship"] = ship;
		configFile["Video"] = video;

		// Save our json file as a new settings.json.
		std::ofstream ofs(GetEXEPath() + "Settings.json");
		ofs << configFile.dump(4);
		ofs.close();
	}
	else
	{
		// Otherwise, a settings file exists and we will process it.

		// Parse our file into our json variable and close the file.
		json settingData = json::parse(ifs);
		ifs.close();

		// Start parsing the data.

		try
		{
			// Video.
			SCREEN_NAME = settingData["Video"]["ScreenName"];
			SCREEN_WIDTH = settingData["Video"]["ScreenWidth"];
			SCREEN_HEIGHT = settingData["Video"]["ScreenHeight"];
			FRAME_RATE = settingData["Video"]["FrameRate"];
			FULLSCREEN = settingData["Video"]["FullScreen"];
			VSYNC = settingData["Video"]["vSync"];

			// Player.
			TURN_RATE = settingData["Ship"]["TurnRate"];
			VEL_INC = settingData["Ship"]["VelocityIncreasePerTick"];
			MAX_VEL = settingData["Ship"]["MaxVelocity"];

			// Bullets.
			BULLET_DISTANCE = settingData["Bullet"]["BulletDistance"];
			BULLET_VELOCITY = settingData["Bullet"]["BulletVelocity"];

			// Asteroids.
			AUTO_SPAWN_ASTEROIDS = settingData["Asteroid"]["AutoSpawnAsteroids"];
			MAX_ASTEROID_VEL = settingData["Asteroid"]["MaxAsteroidVelocity"];
			AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX = settingData["Asteroid"]["AutoSpawnAsteroidsTotalSizeMax"];
			AUTO_SPAWN_ASTEROIDS_TIMER = settingData["Asteroid"]["AutoSpawnAsteroidsTimer"];
			AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER = settingData["Asteroid"]["AutoSpawnAsteroidsDistanceFromPlayer"];

			// Controls.
			controls.fire = settingData["Controls"]["Fire"];
			controls.forwards = settingData["Controls"]["Forwards"];
			controls.backwards = settingData["Controls"]["Backwards"];
			controls.rotateLeft = settingData["Controls"]["RotateLeft"];
			controls.rotateRight = settingData["Controls"]["RotateRight"];
			controls.boost = settingData["Controls"]["Boost"];
			controls.equalize = settingData["Controls"]["Equalize"];
			controls.createAsteroid = settingData["Controls"]["CreateAsteroid"];
		}
		catch (json::exception& e)
		{
			std::string str = e.what();
			debug.Log("GameEngine", "LoadSettings", "Error loading settings file: " + str);
		}
	}
}

// Starts up the SDL engine, loads settings and prepares windows/renderers.
void GameEngine::Init()
{
	if (running)
	{ 
		debug.Log("GameEngine", "Init", "Failed to initialize engine, Engine is already running");
		return;
	}

	debug.showMessagesOnConsole = true;

	debug.Log("GameEngine", "Init", "Starting setup...");

	debug.Log("GameEngine", "Init", "Initializing SDL sub-routines...");

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string str = SDL_GetError();
		debug.Log("GameEngine", "Init", "SDL failed to initialize | " + str);
		return;
	}

	debug.Log("GameEngine", "Init", "Initializing SDL_TTF sub-routines...");

	// Initialize SDL subroutine TTF.
	if (TTF_Init() != 0)
	{
		std::string str = SDL_GetError();

		debug.Log("GameEngine", "Init", "TTF failed to initialize | " + str);
		return;
	}

	debug.Log("GameEngine", "Init", "Initializing SDL_MIXER sub-routines...");

	// Initialize SDL subroutine Audio.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::string str = Mix_GetError();

		debug.Log("GameEngine", "Init", "Mixer failed to initialize | " + str);
		return;
	}

	// Load settings.
	debug.Log("GameEngine", "Init", "Loading settings...");
	LoadSettings();
		
	debug.Log("GameEngine", "Init", "Allocating mixer channels...");
	Mix_AllocateChannels(32);

	// Prepare the renderer.
	debug.Log("GameEngine", "Init", "Preparing renderer...");
	renderer = Renderer();	
	renderer.Init(SCREEN_NAME.c_str(), SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN, VSYNC);

	// Prepare the game state.
	debug.Log("GameEngine", "Init", "Moving to main menu state...");
	PushNewState(new GameState_MainMenu());
	
	debug.Log("GameEngine", "Init", "Completed setup");
	running = true;	
}

// Cleans up and deletes all active states and shuts down the SDL engine.
void GameEngine::Cleanup()
{
	debug.Log("GameEngine", "Cleanup", "Cleaning up engine...");

	debug.Log("GameEngine", "Cleanup", "Cleaning up states...");
	for (auto temp : states)
	{
		temp->Cleanup();
	}

	for (auto temp : states)
	{
		delete temp;
	}

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
	running = State()->HandleInput();
}

// Ask the current GameState to run its HandleEvents code.
void GameEngine::HandleEvents()
{
	State()->HandleEvents();
}

// Ask the current GameState to run its Render code.
void GameEngine::Render()
{
	State()->Render();
}

// Puts the given state onto the top of the state que. (DOES NOT CHECK FOR IDENTICAL STATES)
void GameEngine::PushNewState(GameState* state)
{
	states.push_back(state);
	debug.Log("GameEngine", "PushNewState", "Pushed new state as active.");
}

// Pops the top state from the que.
void GameEngine::PopLastState()
{
	if (states.size() > 0)
	{
		states.back()->Cleanup();
		delete states.back();
		states.pop_back();

		debug.Log("GameEngine", "PopLastState", "Revereted to previous state.");
	}
}

// Checks if we currently have an active state.
bool GameEngine::HasActiveState()
{
	if (State() == nullptr)
		return false;
	else 
		return true;
}

GameEngine game = GameEngine();