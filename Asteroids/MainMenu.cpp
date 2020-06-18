#include "GameState.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"

GameState_MainMenu::GameState_MainMenu()
{
	Init();
}

void GameState_MainMenu::Init()
{
	menuOptionOptions.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionStart.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionQuit.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);

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

	menuOptionStart.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 5) * 2, true);
	menuOptionOptions.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 5) * 3, true);
	menuOptionQuit.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, (game.GetRenderer().WindowHeight() / 5) * 4, true);

	SDL_RenderPresent(game.GetRenderer().renderer);
}
