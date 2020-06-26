#pragma once
#include "GameState.h"

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
	InputManager* iManager = new InputManager();
	
	Textures allTextures;
	TTF menuOptionStart = TTF(nullptr);
	TTF menuOptionOptions = TTF(nullptr);
	TTF menuOptionQuit = TTF(nullptr);
};