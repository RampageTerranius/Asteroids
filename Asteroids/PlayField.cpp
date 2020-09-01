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

	fps = allTTF.CreateTTF(game.GetRenderer().renderer, "fps");

	// Load all the textures the playfield requires.
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 10x10.png", "asteroid 10");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 15x15.png", "asteroid 15");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 20x20.png", "asteroid 20");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 25x25.png", "asteroid 25");

	background = allTextures.CreateTexture(GetEXEPath() + "\\Images\\background.jpg", "background");
	background->centerTextureOnDraw = false;
	background->SetWidthHeight(game.SCREEN_WIDTH, game.SCREEN_HEIGHT);

	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-explosion2.aiff", "explosion");
	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-bump.aiff", "hit");
	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-laser1.aiff", "shot");	

	// Setup player varaibles.
	{
		Chunk* s;
		s = (Chunk*)allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\static.wav", "move");
		s->volume = 20;
		s->allowOverlayingSound = false;
		player.moveSound = s;
	}
	{
		Chunk* s;
		s = (Chunk*)allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\static.wav", "moveBoost");
		s->volume = 50;
		s->allowOverlayingSound = false;
		player.moveBoostSound = s;
	}

	player.tex = allTextures.CreateTexture(GetEXEPath() + "\\images\\player.png", "player");
	player.x = static_cast <float> (game.SCREEN_WIDTH) / 2.0f;
	player.y = static_cast <float> (game.SCREEN_HEIGHT) / 2.0f;
	player.velocity = game.VEL_INC;
	player.turnRate = game.TURN_RATE;
	player.fireInterval = 60;

	// Setup key bindings.
	iManager.Bind(game.controls.fire, commandFire);	
	iManager.Bind(game.controls.forwards, commandForwards);
	iManager.Bind(game.controls.backwards, commandBackwards);
	iManager.Bind(game.controls.rotateLeft, commandRotateLeft);
	iManager.Bind(game.controls.rotateRight, commandRotateRight);
	iManager.Bind(game.controls.boost, commandBoost);
	iManager.Bind(game.controls.equalize, commandEqualizeVelocity);
	iManager.Bind(game.controls.createAsteroid, commandCreateAsteroid);

	fps->SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	fps->CenterImage(false);
	fps->x = 10;
	fps->y = 10;
}

void GameState_PlayField::Cleanup()
{
	allTextures.Cleanup();
	iManager.ClearAll();
	allTTF.ClearAll();
	allSounds.Cleanup();

	delete commandFire;
	delete commandForwards;
	delete commandBackwards;
	delete commandRotateLeft;
	delete commandRotateRight;
	delete commandBoost;
	delete commandEqualizeVelocity;
	delete commandCreateAsteroid;
}

bool GameState_PlayField::HandleInput()
{
	bool running = iManager.GenerateInputAndDispatchCommands();

	// Process the command list as necessary.
	if (!iManager.ProcessCommandList(&player))
		running = false;
	else
	{
		// Check for other input.
		bool justPressedLeft = iManager.JustPressed(SDL_BUTTON_LEFT);
		bool justPressedRight = iManager.JustPressed(SDL_BUTTON_RIGHT);

		if (justPressedLeft || justPressedRight)
		{
			SDL_Point mouseLoc = iManager.GetMouseLocation();

			for (auto asteroid : allAsteroids.allAsteroids)
				if (GetDistance(asteroid->x, asteroid->y, static_cast <float> (mouseLoc.x), static_cast <float> (mouseLoc.y)) <= (asteroid->size / 2))
				{
					if (justPressedLeft)
						asteroid->Break(nullptr);
					else
					{
						Bullet b;
						b.x = player.x;
						b.y = player.y;
						b.startX = player.x;
						b.startY = player.y;
						asteroid->Break(&b);
					}
					break;
				}
		}
	}

	// If the user has signaled to exit we instead want to pop the current state and revert to the last state (generally this means going back to the menu)
	if (!running)
		game.PopLastState();

	return true;
}

void GameState_PlayField::CheckForCollisons()
{
	for (auto asteroid : allAsteroids.allAsteroids)
	{
		// TODO: check for a better way to handle this, recalling CheckForCollisions cant be the optimal way to do this...
		for (auto bullet : allBullets.allBullets)
			if (GetDistance(bullet->x, bullet->y, asteroid->x, asteroid->y) <= (asteroid->size / 2))
			{
				asteroid->Break(bullet);
				bullet->Destroy();
				allSounds.GetSound("hit")->Play();
				debug.Log("Bullet", "Update", "Bullet collided with asteroid");

				// If we broke an asteroid we need to start the function from scratch as data has been both added and removed from vectors.
				// Without starting the function again we can only process one asteroid being destroyed per tick, this should rarely be an issue but must be addressed.
				CheckForCollisons();
				return;
			}

		if (player.immunityTime == 0)
			if (GetDistance(player.x, player.y, asteroid->x, asteroid->y) <= (asteroid->size / 2))
			{
				player.Respawn();
				asteroid->Break(nullptr);
				allSounds.GetSound("explosion")->Play();
				debug.Log("Bullet", "Update", "Asteroid collided with player");
				CheckForCollisons();
				return;
			}
	}
}

void GameState_PlayField::CheckForNewAsteroids()
{
	// Check if we are allowed to potentially create new asteroids.
	if (!game.AUTO_SPAWN_ASTEROIDS)
		return;

	int totalSize = 0;
	for (auto asteroid : allAsteroids.allAsteroids)
		totalSize += asteroid->size;

	// Check if we are below the total size limit of asteroids and if so count down a timer to spawn a new one.
	if (totalSize < game.AUTO_SPAWNED_ASTEROID_TOTAL_SIZE_MAX)
	{
		asteroidAutoSpawnTimer--;

		if (asteroidAutoSpawnTimer <= 0)
		{
			allAsteroids.CreateAsteroid(&player);
			asteroidAutoSpawnTimer = game.AUTO_SPAWN_ASTEROIDS_TIMER;
		}
	}
	else
		asteroidAutoSpawnTimer = game.AUTO_SPAWN_ASTEROIDS_TIMER;
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

	fps->SetText(std::to_string(game.fps));

	background->Draw(game.GetRenderer().renderer, 0, 0);

	player.Draw();
	allBullets.RenderAll();
	allAsteroids.RenderAll();
	allTTF.RenderAll();

	SDL_RenderPresent(game.GetRenderer().renderer);
}