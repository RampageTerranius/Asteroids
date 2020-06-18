#include "GameState.h"
#include "EventHandle.h"
#include "GameEngine.h"

GameState_MainMenu::GameState_MainMenu()
{
	Init();
}

void GameState_MainMenu::Init()
{
	menuOptionOptions.SetFont("\\Fonts\\pxl.ttf", 30);
	menuOptionStart.SetFont("\\Fonts\\pxl.ttf", 30);
	menuOptionQuit.SetFont("\\Fonts\\pxl.ttf", 30);

	menuOptionOptions.SetText(game.GetRenderer().renderer, "Options");
	menuOptionStart.SetText(game.GetRenderer().renderer, "Start");
	menuOptionQuit.SetText(game.GetRenderer().renderer, "Quit");
}

void GameState_MainMenu::Cleanup()
{
	allTextures.Cleanup();

	menuOptionOptions.Clear();
	menuOptionStart.Clear();
	menuOptionQuit.Clear();
}

bool GameState_MainMenu::HandleInput()
{
	std::vector<Command*> CommandList;

	bool quitProgram = this->iManager->GenerateInput(CommandList);

	while (!CommandList.empty())
	{
		//CommandList.back()->Execute(currentPlayer);
		CommandList.pop_back();
	}

	return quitProgram;
}

void GameState_MainMenu::HandleEvents()
{

}

void GameState_MainMenu::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	menuOptionStart.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 4) * 1);
	menuOptionOptions.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 4) * 2);
	menuOptionQuit.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 4) * 3);

	SDL_RenderPresent(game.GetRenderer().renderer);
}
