#include <SDL.h>

#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Debug.h"

// Constructor.
GameState_PlayField::GameState_PlayField()
{	
	Init();	
}

// Initialize all data required for the play field.
void GameState_PlayField::Init()
{	
	// Make sure our vectors are ready for use.
	allAsteroids.Clear();
	allBullets.Clear();

	// Set some values to their default states.
	score = 0;
	asteroidAutoSpawnTimer = 0;

	// Load all the textures the playfield requires.
	allTextures.CreateTexture(GetEXEPath() + "Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "Images\\asteroid 10x10.png", "asteroid 10");
	allTextures.CreateTexture(GetEXEPath() + "Images\\asteroid 15x15.png", "asteroid 15");
	allTextures.CreateTexture(GetEXEPath() + "Images\\asteroid 20x20.png", "asteroid 20");
	allTextures.CreateTexture(GetEXEPath() + "Images\\asteroid 25x25.png", "asteroid 25");

	// The background needs special attention as it needs to be stretched across the entire screen
	background = allTextures.CreateTexture(GetEXEPath() + "Images\\background.jpg", "background");
	background->anchor = Anchor::TopLeft;
	background->SetWidthHeight(game.SCREEN_WIDTH, game.SCREEN_HEIGHT);

	// Create our sounds
	allSounds.CreateChunk(GetEXEPath() + "Sounds\\8-bit-explosion2.aiff", "explosion");
	allSounds.CreateChunk(GetEXEPath() + "Sounds\\8-bit-bump.aiff", "hit");
	allSounds.CreateChunk(GetEXEPath() + "Sounds\\8-bit-laser1.aiff", "shot");	

	// Create and assign sounds for the player.
	{
		Chunk* temp = allSounds.CreateChunk(GetEXEPath() + "Sounds\\static.wav", "move");
		temp->volume = 20;
		temp->allowOverlayingSound = false;
		player.moveForwardsSound = temp;
	}	
	{
		Chunk* temp = allSounds.CreateChunk(GetEXEPath() + "Sounds\\static.wav", "moveBoost");
		temp->volume = 40;
		temp->allowOverlayingSound = false;
		player.moveForwardsBoostSound = temp;
	}
	{
		Chunk* temp = allSounds.CreateChunk(GetEXEPath() + "Sounds\\static2.wav", "moveBack");
		temp->volume = 20;
		temp->allowOverlayingSound = false;
		player.moveBackwardsSound = temp;
	}
	{
		Chunk* temp = allSounds.CreateChunk(GetEXEPath() + "Sounds\\static2.wav", "moveBackBoost");
		temp->volume = 40;
		temp->allowOverlayingSound = false;
		player.moveBackwardsBoostSound = temp;
	}
	{
		Chunk* temp = allSounds.CreateChunk(GetEXEPath() + "Sounds\\static3.wav", "equalize");
		temp->volume = 20;
		temp->allowOverlayingSound = false;
		player.equalizeSound = temp;
	}

	// Setup the players data.
	player.tex = allTextures.CreateTexture(GetEXEPath() + "images\\player.png", "player");
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


	// Create TTF objects for fps and player score.
	fps = allTTF.CreateTTF(game.GetRenderer().renderer, "fps");
	fps->SetFont(GetEXEPath() + "Fonts\\pxl.ttf", 30);
	fps->SetAnchor(Anchor::TopLeft);
	fps->x = 10;
	fps->y = 10;
	
	score = allTTF.CreateTTF(game.GetRenderer().renderer, "score");
	score->SetFont(GetEXEPath() + "Fonts\\pxl.ttf", 30);
	score->SetAnchor(Anchor::TopRight);
	score->y = 10;
	score->x = game.SCREEN_WIDTH - 10;
}

// Cleanup all data for this game state.
void GameState_PlayField::Cleanup()
{
	// Let each manager handle their cleanup.
	allTextures.Cleanup();
	iManager.ClearAll();	
	allSounds.Cleanup();
	allTTF.ClearAll();

	// Clear all commands by hand.
	delete commandFire;
	delete commandForwards;
	delete commandBackwards;
	delete commandRotateLeft;
	delete commandRotateRight;
	delete commandBoost;
	delete commandEqualizeVelocity;
	delete commandCreateAsteroid;
}

// Handle input for the gamestate.
bool GameState_PlayField::HandleInput()
{
	// Have the input manager generate input AND dispatch commands.
	bool running = iManager.GenerateInputAndDispatchCommands();

	// Process the command list as necessary.
	if (!iManager.ProcessCommandList(&player))
	{
		running = false;
	}
	else
	{
		// Check for other input.
		bool justPressedLeft = iManager.JustPressed(SDL_BUTTON_LEFT);
		bool justPressedRight = iManager.JustPressed(SDL_BUTTON_RIGHT);

		// If a user clicks on a meteor break it (debug purposes)
		if (justPressedLeft || justPressedRight)
		{
			SDL_Point mouseLoc = iManager.GetMouseLocation();

			for (auto asteroid : allAsteroids.allAsteroids)
			{
				if (GetDistance(asteroid->x, asteroid->y, static_cast <float> (mouseLoc.x), static_cast <float> (mouseLoc.y)) <= (asteroid->size / 2))
				{
					if (justPressedLeft)
					{
						asteroid->Break(nullptr);
					}
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
	}

	// If the user has signaled to exit we instead want to pop the current state and revert to the last state (generally this means going back to the menu)
	if (!running)
	{
		game.PopLastState();
	}

	return true;
}

// Check if there has been any collisons between player/asteroid or asteroid/bullet.
void GameState_PlayField::CheckForCollisons()
{
	for (auto asteroid : allAsteroids.allAsteroids)
	{
		// TODO: check for a better way to handle this, recalling CheckForCollisions cant be the optimal way to do this...
		for (auto bullet : allBullets.allBullets)
		{
			if (GetDistance(bullet->x, bullet->y, asteroid->x, asteroid->y) <= (asteroid->size / 2))
			{
				currentScore += game.BULLET_DISTANCE - bullet->distanceLeft;
				asteroid->Break(bullet);
				bullet->Destroy();
				allSounds.GetSound("hit")->Play();
				debug.Log("Bullet", "Update", "Bullet collided with asteroid");
				return;
			}

			if (player.immunityTime == 0)
				if (GetDistance(player.x, player.y, asteroid->x, asteroid->y) <= (asteroid->size / 2))
				{
					player.Respawn();
					asteroid->Break(nullptr);
					allSounds.GetSound("explosion")->Play();
					debug.Log("Bullet", "Update", "Asteroid collided with player");
					return;
				}
		}
	}
}

void GameState_PlayField::CheckForNewAsteroids()
{
	// Check if we are allowed to potentially create new asteroids.
	if (!game.AUTO_SPAWN_ASTEROIDS)
	{
		return;
	}

	int totalSize = 0;
	for (auto asteroid : allAsteroids.allAsteroids)
	{
		totalSize += asteroid->size;
	}

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
	{
		asteroidAutoSpawnTimer = game.AUTO_SPAWN_ASTEROIDS_TIMER;
	}
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
	score->SetText(std::to_string(currentScore));

	background->Draw(game.GetRenderer().renderer, 0, 0);

	player.Draw();
	allBullets.RenderAll();
	allAsteroids.RenderAll();
	allTTF.RenderAll();

	SDL_RenderPresent(game.GetRenderer().renderer);
}