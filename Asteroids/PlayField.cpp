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
}

void GameState_PlayField::Cleanup()
{
	allTextures.Cleanup();
}

bool GameState_PlayField::HandleInput()
{
	bool running = this->iManager->GenerateInput();

	// If the user has signaled to exit we instead want to pop the current state and revert to the last state (generally this means going back to the menu)
	if (!running)
		game.PopLastState();

	return true;
}

void GameState_PlayField::HandleEvents()
{

}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	//player.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}