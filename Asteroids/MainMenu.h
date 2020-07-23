#pragma once
#include "GameState.h"

enum class MenuOption
{
	none,
	start,
	options,
	quit,
	totalCount // Used to determine the total size of the enum, always have this as the last.
};

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
	TTF menuOptionStart = TTF(nullptr);
	TTF menuOptionOptions = TTF(nullptr);
	TTF menuOptionQuit = TTF(nullptr);

	MenuOption menuOption;	
};