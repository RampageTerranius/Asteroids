#include <SDL.h>

#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Debug.h"

GameState_PlayField::GameState_PlayField()
{	
	Init();	
}

void GameState_PlayField::Init()
{	
	allAsteroids.Clear();
	allBullets.Clear();

	this->fps = TTF(game.GetRenderer().renderer);

	// Load all the textures the playfield requires.
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 10x10.png", "asteroid 10");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 15x15.png", "asteroid 15");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 20x20.png", "asteroid 20");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 25x25.png", "asteroid 25");

	// Setup player varaibles.
	this->player.tex = allTextures.CreateTexture(GetEXEPath() + "\\images\\player.png", "player");
	this->player.x = static_cast <float> (game.SCREEN_WIDTH) / 2.0f;
	this->player.y = static_cast <float> (game.SCREEN_HEIGHT) / 2.0f;
	this->player.velocity = game.VEL_INC;
	this->player.turnRate = game.TURN_RATE;
	this->player.fireInterval = 60;

	// Setup key bindings.
	this->iManager->Bind(SDLK_SPACE, this->commandFire);	
	this->iManager->Bind(SDLK_w, this->commandForwards);
	this->iManager->Bind(SDLK_s, this->commandBackwards);
	this->iManager->Bind(SDLK_a, this->commandRotateLeft);
	this->iManager->Bind(SDLK_d, this->commandRotateRight);
	this->iManager->Bind(SDLK_LSHIFT, this->commandBoost);
	this->iManager->Bind(SDLK_c, this->commandEqualizeVelocity);
	this->iManager->Bind(SDLK_f, this->commandCreateAsteroid);

	fps.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	fps.CenterImage(false);
	fps.x = 10;
	fps.y = 10;
}

void GameState_PlayField::Cleanup()
{

	this->allTextures.Cleanup();

	delete this->commandFire;
	this->commandFire = nullptr;

	delete this->commandForwards;
	this->commandForwards = nullptr;

	delete this->commandBackwards;
	this->commandBackwards = nullptr;

	delete this->commandRotateLeft;
	this->commandRotateLeft = nullptr;

	delete this->commandRotateRight;
	this->commandRotateRight = nullptr;

	delete this->commandBoost;
	this->commandBoost = nullptr;

	delete this->commandEqualizeVelocity;
	this->commandEqualizeVelocity = nullptr;

	delete this->commandCreateAsteroid;
	this->commandCreateAsteroid = nullptr;
}

bool GameState_PlayField::HandleInput()
{
	bool running = this->iManager->GenerateInputAndDispatchCommands();

	if (!this->iManager->ProcessCommandList(&this->player))
		running = false;

	// If the user has signaled to exit we instead want to pop the current state and revert to the last state (generally this means going back to the menu)
	if (!running)
		game.PopLastState();

	return true;
}

void GameState_PlayField::CheckForCollisons()
{
	for (auto asteroid : allAsteroids.allAsteroids)
	{
		for (auto bullet : allBullets.allBullets)
			if (GetDistance(bullet->x, asteroid->x, bullet->y, asteroid->y) <= (asteroid->size / 2))
			{
				asteroid->Break();
				bullet->Destroy();

				debug.Log("Bullet", "Update", "Bullet collided with asteroid");

				// If we broke an asteroid we need to start the function from scratch as data has been both added and removed from vectors.
				// Without starting the function again we can only process one asteroid being destroyed per tick, this should rarely be an issue but must be addressed.
				CheckForCollisons();
				return;
			}

		if (player.immunityTime == 0)
			if (GetDistance(this->player.x, asteroid->x, this->player.y, asteroid->y) <= (asteroid->size / 2))
			{
				// Kill player here.
				player.Respawn();

				debug.Log("Bullet", "Update", "Asteroid collided with player");
			}
	}
}

void GameState_PlayField::CheckForNewAsteroids()
{
	int totalSize = 0;
	for (auto asteroid : allAsteroids.allAsteroids)
		totalSize += asteroid->size;

	if (totalSize < game.AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX)
	{
		this->asteroidAutoSpawnTimer--;

		if (this->asteroidAutoSpawnTimer <= 0)
		{
			allAsteroids.CreateAsteroid(&this->player);
			this->asteroidAutoSpawnTimer = game.AUTO_SPAWN_ASTEROIDS_TIMER;
		}
	}
	else
		this->asteroidAutoSpawnTimer = game.AUTO_SPAWN_ASTEROIDS_TIMER;
}

void GameState_PlayField::HandleEvents()
{
	// Handle players events.
	player.Update();

	allBullets.UpdateAll();

	allAsteroids.UpdateAll();

	CheckForCollisons();

	CheckForNewAsteroids();
}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	player.Draw();

	allBullets.RenderAll();

	allAsteroids.RenderAll();

	fps.SetText(std::to_string(game.fps));
	fps.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}