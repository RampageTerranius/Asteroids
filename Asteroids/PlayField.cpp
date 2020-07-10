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
	player.x = game.SCREEN_WIDTH / 2;
	player.y = game.SCREEN_HEIGHT / 2;
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

void GameState_PlayField::CalcPlayerMovement()
{

}

void GameState_PlayField::HandleEvents()
{
	// Handle players events.
	player.Update();
}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	player.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}