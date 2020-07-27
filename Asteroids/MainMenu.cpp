#include "MainMenu.h"
#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Command.h"
#include "Debug.h"

#include "Random.h"

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

	menuOptionStart.CenterImage(true);
	menuOptionOptions.CenterImage(true);
	menuOptionQuit.CenterImage(true);	
}

void GameState_MainMenu::Cleanup()
{
	allTextures.Cleanup();
	iManager.ClearAll();

	menuOptionOptions.Clear();
	menuOptionStart.Clear();
	menuOptionQuit.Clear();
}

bool GameState_MainMenu::HandleInput()
{
	bool running = iManager.GenerateInput();

	// Player has asked to goto next state.
	if (iManager.JustPressed(SDL_BUTTON_LEFT) || iManager.JustPressed(SDLK_SPACE))
		switch (menuOption)
		{
		case MenuOption::options:
			break;

		case MenuOption::quit:
			return false;
			break;

		case MenuOption::start:
			while (true)
			{
				game.PushNewState(new GameState_PlayField());
				game.PopLastState();
			}
			break;
		}

	// On pressing down go to next menu option.
	if (iManager.JustPressed(SDLK_s) || iManager.JustPressed(SDLK_DOWN))
	{
		int i = static_cast <int> (menuOption);
		i++;
		if (i >= static_cast <int> (MenuOption::totalCount))
			i = 1;
		menuOption = static_cast <MenuOption> (i);
	}

	// On pressing up go to the previous menu option.
	if (iManager.JustPressed(SDLK_w) || iManager.JustPressed(SDLK_UP))
	{
		int i = static_cast <int> (menuOption);
		i--;
		if (i < 1)
			i = static_cast <int> (MenuOption::totalCount) - 1;

		menuOption = static_cast <MenuOption> (i);
	}

	return running;
}

void GameState_MainMenu::HandleEvents()
{
	SDL_Point point = iManager.GetMouseLocation();	

	if (menuOptionOptions.PointIntersectsTexture(point))	
		menuOption = MenuOption::options;

	if (menuOptionQuit.PointIntersectsTexture(point))	
		menuOption = MenuOption::quit;	

	if (menuOptionStart.PointIntersectsTexture(point))	
		menuOption = MenuOption::start;
}

void GameState_MainMenu::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	menuOptionOptions.SetColor(255, 255, 255);
	menuOptionQuit.SetColor(255, 255, 255);
	menuOptionStart.SetColor(255, 255, 255);

	switch (menuOption)
	{
	case MenuOption::options:
		menuOptionOptions.SetColor(0, 0, 200);
		break;

	case MenuOption::quit:
		menuOptionQuit.SetColor(0, 0, 200);
		break;

	case MenuOption::start:
		menuOptionStart.SetColor(0, 0, 200);
		break;
	}

	menuOptionStart.Draw();
	menuOptionOptions.Draw();
	menuOptionQuit.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}