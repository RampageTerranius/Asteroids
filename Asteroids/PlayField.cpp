#include <SDL.h>

#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"

GameState_PlayField::GameState_PlayField()
{
	Init();	
}

void GameState_PlayField::Init()
{	
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");

	this->player.tex = allTextures.CreateTexture(GetEXEPath() + "images\\player.png", "player");
	this->player.x = game.SCREEN_WIDTH / 2;
	this->player.y = game.SCREEN_HEIGHT / 2;
	this->player.velocity = game.VEL_INC;
	this->player.turnRate = game.TURN_RATE;
	this->player.fireInterval = 60;
	this->player.centerTexture = true;

	this->iManager->Bind(SDLK_SPACE, this->commandFire);	
	this->iManager->Bind(SDLK_w, this->commandForwards);
	this->iManager->Bind(SDLK_s, this->commandBackwards);
	this->iManager->Bind(SDLK_a, this->commandRotateLeft);
	this->iManager->Bind(SDLK_d, this->commandRotateRight);
	this->iManager->Bind(SDLK_LSHIFT, this->commandBoost);
	this->iManager->Bind(SDLK_c, this->commandEqualizeVelocity);
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

	SDL_RenderPresent(game.GetRenderer().renderer);
}