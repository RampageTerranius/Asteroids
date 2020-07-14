#include <SDL.h>

#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"

GameState_PlayField::GameState_PlayField()
{
	fps = TTF(game.GetRenderer().renderer);
	Init();	
}

void GameState_PlayField::Init()
{	
	// Load all the textures the playfield requires.
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 5x5.png", "asteroid 5");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 10x10.png", "asteroid 10");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 15x15.png", "asteroid 15");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 20x20.png", "asteroid 20");

	// Setup player varaibles.
	this->player.tex = allTextures.CreateTexture(GetEXEPath() + "\\images\\player.png", "player");
	this->player.x = game.SCREEN_WIDTH / 2;
	this->player.y = game.SCREEN_HEIGHT / 2;
	this->player.velX = 1;
	this->player.velocity = game.VEL_INC;
	this->player.turnRate = game.TURN_RATE;
	this->player.fireInterval = 60;
	this->player.centerTexture = true;

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

void GameState_PlayField::HandleEvents()
{
	// Handle players events.
	player.Update();

	allBullets.UpdateAll();

	allAsteroids.UpdateAll();
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