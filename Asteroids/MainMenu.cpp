#include "MainMenu.h"
#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Command.h"
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
	menuOptionStart.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionOptions.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);	
	menuOptionQuit.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);

	menuOptionStart.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionStart.y = (game.GetRenderer().WindowHeight() / 5) * 2;
	menuOptionOptions.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionOptions.y = (game.GetRenderer().WindowHeight() / 5) * 3;
	menuOptionQuit.x = game.GetRenderer().WindowWidth() / 2;
	menuOptionQuit.y = (game.GetRenderer().WindowHeight() / 5) * 4;

	menuOptionStart.SetText("Start");
	menuOptionOptions.SetText("Options");	
	menuOptionQuit.SetText("Quit");

	menuOptionStart.centerImage = true;
	menuOptionOptions.centerImage = true;	
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

	if (iManager->IsHeld(SDL_BUTTON_LEFT))	
		switch (game.menuOption)
		{
		case 1:// options

			break;

		case 2:// quit
			return false;
			break;

		case 3:// start
			game.PushNewState(new GameState_PlayField());
			break;
		}	

	return running;
}

void GameState_MainMenu::HandleEvents()
{
	game.menuOption = 0;	

	SDL_Point point = iManager->GetMouseLocation();	

	if (menuOptionOptions.PointIntersectsTexture(point))
	{
		game.menuOption = 1;
		menuOptionOptions.SetColor(0, 0, 200);
	}
	else
		menuOptionOptions.SetColor(255, 255, 255);

	if (menuOptionQuit.PointIntersectsTexture(point))
	{
		game.menuOption = 2;
		menuOptionQuit.SetColor(0, 0, 200);
	}
	else
		menuOptionQuit.SetColor(255, 255, 255);

	if (menuOptionStart.PointIntersectsTexture(point))
	{
		game.menuOption = 3;
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
