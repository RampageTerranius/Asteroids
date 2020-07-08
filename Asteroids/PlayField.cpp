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
	bool running = this->iManager->GenerateInput(this->iManager->commandList);

	while (!this->iManager->commandList.empty())
	{
		if (!this->iManager->commandList.back()->Execute())
		{
			running = false;
			break;
		}
		if (this->iManager->commandList.size() > 0)
			this->iManager->commandList.pop_back();
	}

	// If the user has signeld to exit we instead want to pop the current state and revert to the last state (generally this means gonig back to the menu)
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

	SDL_RenderPresent(game.GetRenderer().renderer);
}