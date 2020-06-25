#include "GameState.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Debug.h"

GameState_MainMenu::GameState_MainMenu()
{
	menuOptionStart = TTF(game.GetRenderer().renderer);
	menuOptionOptions = TTF(game.GetRenderer().renderer);
	menuOptionQuit = TTF(game.GetRenderer().renderer);
	Init();
}

void GameState_MainMenu::Init()
{
	menuOptionOptions.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionStart.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionQuit.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);

	menuOptionOptions.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionOptions.y = (game.GetRenderer().WindowHeight() / 5) * 2;
	menuOptionStart.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionStart.y = (game.GetRenderer().WindowHeight() / 5) * 3;
	menuOptionQuit.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionQuit.y = (game.GetRenderer().WindowHeight() / 5) * 4;

	menuOptionOptions.SetText("Options");
	menuOptionStart.SetText("Start");
	menuOptionQuit.SetText("Quit");

	menuOptionOptions.centerImage = true;
	menuOptionStart.centerImage = true;
	menuOptionQuit.centerImage = true;
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
	selectedOption = 0;	

	SDL_Point point = iManager->GetMouseLocation();	

	if (menuOptionOptions.PointInterectsImage(point))
	{
		selectedOption = 1;
		menuOptionOptions.SetColor(0, 0, 200);
	}
	else
		menuOptionOptions.SetColor(255, 255, 255);

	if (menuOptionQuit.PointInterectsImage(point))
	{
		selectedOption = 2;
		menuOptionQuit.SetColor(0, 0, 200);
	}
	else
		menuOptionQuit.SetColor(255, 255, 255);

	if (menuOptionStart.PointInterectsImage(point))
	{
		selectedOption = 3;
		menuOptionStart.SetColor(0, 0, 200);
	}
	else
		menuOptionStart.SetColor(255, 255, 255);
}

void GameState_MainMenu::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	menuOptionStart.Draw();
	menuOptionOptions.Draw();
	menuOptionQuit.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}
