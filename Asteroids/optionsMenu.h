#pragma once
#include "GameState.h"

// Game state for the options menu.
// NOT CURRENTLY IMPLEMENTED.
class GameState_Options : public GameState
{
public:
	GameState_Options();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

private:	
	TTF playFieldTest = TTF(nullptr);
};