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
	player.tex = allTextures.CreateTexture(GetEXEPath() + "images\\player.png", "player");
	player.x = 400;
	player.y = 400;
	player.velocity = game.VEL_INC;
	player.turnRate = game.TURN_RATE;

	this->iManager->Bind(SDLK_w, new CommandMoveForward());
	this->iManager->Bind(SDLK_s, new CommandMoveBackwards());
	this->iManager->Bind(SDLK_a, new CommandMoveLeft());
	this->iManager->Bind(SDLK_d, new CommandMoveRight());
}

void GameState_PlayField::Cleanup()
{
	allTextures.Cleanup();
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
	// Handle movement of player first.
	if (this->player.velX > game.MAX_VEL)
		this->player.velX = game.MAX_VEL;
	if (this->player.velX < -game.MAX_VEL)
		this->player.velX = -game.MAX_VEL;

	if (this->player.velY > game.MAX_VEL)
		this->player.velY = game.MAX_VEL;
	if (this->player.velY < -game.MAX_VEL)
		this->player.velY = -game.MAX_VEL;

	this->player.x += this->player.velX;
	this->player.y += this->player.velY;

	if (this->player.x < 0)
		this->player.x = 0;
	else if (this->player.x > game.SCREEN_WIDTH)
		this->player.x = static_cast <float> (game.SCREEN_WIDTH);

	if (this->player.y < 0)
		this->player.y = 0;
	else if (this->player.y > game.SCREEN_HEIGHT)
		this->player.y = static_cast <float> (game.SCREEN_HEIGHT);

	// Handle players events.
	player.Update();
}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	player.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}