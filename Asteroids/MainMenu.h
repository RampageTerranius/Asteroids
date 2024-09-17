#pragma once
#include "GameState.h"
#include "TTF.h"

// Enum for the currently selected option in the menu.
enum class MenuOption
{
	none,
	start,
	options,
	quit,
	totalCount // Used to determine the total size of the enum, always have this as the last.
};

// Game state for when the user is in the main menu.
// Simply shows a list of options allowign the user to either play/goto options/quit.
class GameState_MainMenu : public GameState
{
public:
	GameState_MainMenu();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

private:
	TTFs allTTF;
	MenuOption menuOption;// Currently selected menu option.

	// TTF pointers for direct access.
	TTF* menuOptionOptions;
	TTF* menuOptionQuit;
	TTF* menuOptionStart;
};