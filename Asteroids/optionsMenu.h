#pragma once
#include "GameState.h"

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
	Textures allTextures;
	TTF playFieldTest = TTF(nullptr);
};