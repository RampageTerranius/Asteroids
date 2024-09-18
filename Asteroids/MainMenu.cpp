#include "MainMenu.h"

#include "Game_Engine.h"
#include "PlayField.h"
#include "Misc_Functions.h"

// Constructor.
GameState_MainMenu::GameState_MainMenu()
{
	Init();
}

// Initialize data for this gamestate.
void GameState_MainMenu::Init()
{
	// Have the TTF list create and store our buttons.
	menuOptionStart = allTTF.CreateTTF(game.GetRenderer().renderer, "start", GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionOptions = allTTF.CreateTTF(game.GetRenderer().renderer, "options", GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	menuOptionQuit = allTTF.CreateTTF(game.GetRenderer().renderer, "quit", GetEXEPath() + "\\Fonts\\pxl.ttf", 30);

	// Set the location for each button.
	menuOptionStart->x = game.GetRenderer().WindowWidth() / 2;
	menuOptionStart->y = (game.GetRenderer().WindowHeight() / 5) * 2;
	menuOptionOptions->x = game.GetRenderer().WindowWidth() / 2;
	menuOptionOptions->y = (game.GetRenderer().WindowHeight() / 5) * 3;
	menuOptionQuit->x = game.GetRenderer().WindowWidth() / 2;
	menuOptionQuit->y = (game.GetRenderer().WindowHeight() / 5) * 4;

	// Set the text.
	menuOptionStart->SetText("Start");
	menuOptionOptions->SetText("Options");
	menuOptionQuit->SetText("Quit");

	// Anchor each button to the middle of their location.
	menuOptionStart->SetAnchor(Anchor::Center);
	menuOptionOptions->SetAnchor(Anchor::Center);
	menuOptionQuit->SetAnchor(Anchor::Center);
}

// Cleanup all data in use by this state.
void GameState_MainMenu::Cleanup()
{
	// As we are using managers, have the managers handle their own cleanup.
	allTextures.Cleanup();	
	iManager.ClearAll();
	allSounds.Cleanup();

	allTTF.ClearAll();
}

// Handle incoming input for this game state.
bool GameState_MainMenu::HandleInput()
{
	// Have the input manager generate input.
	bool running = iManager.GenerateInput();

	// If player has jsut pressed either space or left mouse check if we have any options selected.
	if (iManager.JustPressed(SDL_BUTTON_LEFT) || iManager.JustPressed(SDLK_SPACE))
		switch (menuOption)
		{
		case MenuOption::options:
			break;

		case MenuOption::quit:
			return false;
			break;

		case MenuOption::start:
			game.PushNewState(new GameState_PlayField());
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

// Handle events for the current game state.
void GameState_MainMenu::HandleEvents()
{
	// Get the mouse location and determine if any buttons are highlighted.
	SDL_Point point = iManager.GetMouseLocation();

	if (allTTF.GetTTF("options")->PointIntersectsTexture(point))	
		menuOption = MenuOption::options;

	if (allTTF.GetTTF("quit")->PointIntersectsTexture(point))
		menuOption = MenuOption::quit;	

	if (allTTF.GetTTF("start")->PointIntersectsTexture(point))
		menuOption = MenuOption::start;
}

// Render the current game state to teh screen
void GameState_MainMenu::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	// Set the base color for the buttons when not selected.
	menuOptionOptions->SetColor(255, 255, 255);
	menuOptionQuit->SetColor(255, 255, 255);
	menuOptionStart->SetColor(255, 255, 255);

	// If any of the buttons ARE selected, change their color.
	switch (menuOption)
	{
	case MenuOption::options:
		menuOptionOptions->SetColor(0, 0, 200);
		break;

	case MenuOption::quit:
		menuOptionQuit->SetColor(0, 0, 200);
		break;

	case MenuOption::start:
		menuOptionStart->SetColor(0, 0, 200);
		break;
	}

	// Push all TTF objects to the renderer and render the screen as is.
	allTTF.RenderAll();
	SDL_RenderPresent(game.GetRenderer().renderer);
}