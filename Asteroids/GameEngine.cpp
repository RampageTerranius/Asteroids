#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <fstream>

#include "GameEngine.h"
#include "MainMenu.h"
#include "PlayField.h"
#include "Misc Functions.h"
#include "Debug.h"

#include "SimpleINI/SimpleINI.h"

GameEngine::GameEngine()
{	
	Init();	
}

// Loads the settings for the program.
void GameEngine::LoadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error error = ini.LoadFile((GetEXEPath() + "Settings.ini").c_str());
	if (error < 0)
	{
		ini.SetValue("Video", "ScreenName", SCREEN_NAME.c_str());
		ini.SetValue("Video", "ScreenWidth", std::to_string(SCREEN_WIDTH).c_str());
		ini.SetValue("Video", "ScreenHeight", std::to_string(SCREEN_HEIGHT).c_str());
		ini.SetValue("Video", "FrameRate", std::to_string(FRAME_RATE).c_str());
		ini.SetBoolValue("Video", "FullScreen", false);
		ini.SetBoolValue("Video", "vSync", false);

		ini.SetDoubleValue("Ship", "TurnRate", TURN_RATE);
		ini.SetDoubleValue("Ship", "VelocityIncreasePerTick", VEL_INC);
		ini.SetDoubleValue("Ship", "MaxVelocity", MAX_VEL);

		ini.SetValue("Bullet", "BulletDistance", std::to_string(BULLET_DISTANCE).c_str());
		ini.SetDoubleValue("Bullet", "BulletVolocity", BULLET_VELOCITY);

		ini.SetBoolValue("Asteroid", "AutoSpawnAsteroids", true);
		ini.SetDoubleValue("Asteroid", "MaxAsteroidVelocity", MAX_ASTEROID_VEL);
		ini.SetValue("Asteroid", "AutoSpawnAsteroidsTotalSizeMax", std::to_string(AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX).c_str());
		ini.SetValue("Asteroid", "AutoSpawnAsteroidsTimer", std::to_string(AUTO_SPAWN_ASTEROIDS_TIMER).c_str());
		ini.SetDoubleValue("Asteroid", "AutoSpawnAsteroidsDistanceFromPlayer", AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER);

		ini.SetValue("Controls", "Fire", std::to_string(SDLK_SPACE).c_str());
		ini.SetValue("Controls", "Forwards", std::to_string(SDLK_w).c_str());
		ini.SetValue("Controls", "Backwards", std::to_string(SDLK_s).c_str());
		ini.SetValue("Controls", "RotateLeft", std::to_string(SDLK_a).c_str());
		ini.SetValue("Controls", "RotateRight", std::to_string(SDLK_d).c_str());
		ini.SetValue("Controls", "Boost", std::to_string(SDLK_LSHIFT).c_str());
		ini.SetValue("Controls", "Equalize", std::to_string(SDLK_c).c_str());
		ini.SetValue("Controls", "CreateAsteroid", std::to_string(SDLK_f).c_str());

		ini.SaveFile((GetEXEPath() + "Settings.ini").c_str());
	}
	else
	{
		// Load each variable.
		// Video.
		SCREEN_NAME = ini.GetValue("Video", "ScreenName", SCREEN_NAME.c_str());

		try
		{
			SCREEN_WIDTH = std::stoi(ini.GetValue("Video", "ScreenWidth", std::to_string(SCREEN_WIDTH).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value ScreenWidth, defaulting to " + std::to_string(SCREEN_WIDTH));
		}

		try
		{
			SCREEN_HEIGHT = std::stoi(ini.GetValue("Video", "ScreenHeight", std::to_string(SCREEN_HEIGHT).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value ScreenHeight, defaulting to " + std::to_string(SCREEN_HEIGHT));
		}

		try
		{
			FRAME_RATE = std::stoi(ini.GetValue("Video", "FrameRate", std::to_string(FRAME_RATE).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value FrameRate, defaulting to " + std::to_string(FRAME_RATE));
		}

		FULLSCREEN = ini.GetBoolValue("Video", "FullScreen", false);
		VSYNC = ini.GetBoolValue("Video", "vSync", false);

		// Player.
		TURN_RATE = static_cast <float> (ini.GetDoubleValue("Ship", "TurnRate", TURN_RATE));
		VEL_INC = static_cast <float> (ini.GetDoubleValue("Ship", "VelocityIncreasePerTick", VEL_INC));
		MAX_VEL = static_cast <float> (ini.GetDoubleValue("Ship", "MaxVelocity", MAX_VEL));

		// Bullets.
		try
		{
			BULLET_DISTANCE = std::stoi(ini.GetValue("Bullet", "BulletDistance", std::to_string(BULLET_DISTANCE).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value BulletDistance, defaulting to " + std::to_string(BULLET_DISTANCE));
		}

		BULLET_VELOCITY = static_cast <float> (ini.GetDoubleValue("Bullet", "BulletVolocity", BULLET_VELOCITY));

		// Asteroids.
		AUTO_SPAWN_ASTEROIDS = ini.GetBoolValue("Asteroid", "AutoSpawnAsteroids", true);

		MAX_ASTEROID_VEL = static_cast <float> (ini.GetDoubleValue("Asteroid", "MaxAsteroidVelocity", MAX_ASTEROID_VEL));

		try
		{
			AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX = std::stoi(ini.GetValue("Asteroid", "AutoSpawnAsteroidsTotalSizeMax", std::to_string(AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value AutoSpawnAsteroidsTotalSizeMax, defaulting to " + std::to_string(AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX));
		}

		try
		{
			AUTO_SPAWN_ASTEROIDS_TIMER = std::stoi(ini.GetValue("Asteroid", "AutoSpawnAsteroidsTimer", std::to_string(AUTO_SPAWN_ASTEROIDS_TIMER).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value AutoSpawnAsteroidsTimer, defaulting to " + std::to_string(AUTO_SPAWN_ASTEROIDS_TIMER));
		}

		AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER = static_cast <float> (ini.GetDoubleValue("Asteroid", "AutoSpawnAsteroidsDistanceFromPlayer", AUTO_SPAWN_ASTEROIDS_DISTANCE_FROM_PLAYER));

		// Controls.		
		try
		{
			controls.fire = std::stoi(ini.GetValue("Controls", "Fire", std::to_string(SDLK_SPACE).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value Fire, defaulting to " + std::to_string(SDLK_SPACE));
		}

		try
		{
			controls.forwards = std::stoi(ini.GetValue("Controls", "Forwards", std::to_string(SDLK_w).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value Forwards, defaulting to " + std::to_string(SDLK_w));
		}

		try
		{
			controls.backwards = std::stoi(ini.GetValue("Controls", "Backwards", std::to_string(SDLK_s).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value Backwards, defaulting to " + std::to_string(SDLK_s));
		}

		try
		{
			controls.rotateLeft = std::stoi(ini.GetValue("Controls", "RotateLeft", std::to_string(SDLK_a).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value RotateLeft, defaulting to " + std::to_string(SDLK_a));
		}

		try
		{
			controls.rotateRight = std::stoi(ini.GetValue("Controls", "RotateRight", std::to_string(SDLK_d).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value RotateRight, defaulting to " + std::to_string(SDLK_d));
		}

		try
		{
			controls.boost = std::stoi(ini.GetValue("Controls", "Boost", std::to_string(SDLK_LSHIFT).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value Boost, defaulting to " + std::to_string(SDLK_LSHIFT));
		}

		try
		{
			controls.equalize = std::stoi(ini.GetValue("Controls", "Equalize", std::to_string(SDLK_c).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value Equalize, defaulting to " + std::to_string(SDLK_c));
		}

		try
		{
			controls.createAsteroid = std::stoi(ini.GetValue("Controls", "CreateAsteroid", std::to_string(SDLK_f).c_str()));
		}
		catch (const std::exception&)
		{
			debug.Log("GameEngine", "Init", "Failed to convert value CreateAsteroid, defaulting to " + std::to_string(SDLK_f));
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
	for (auto var : states)
		var->Cleanup();

	for (auto var : states)
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