#pragma once
#include "GameState.h"

class GameState_PlayField : public GameState
{
public:
	GameState_PlayField();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

private:
	Textures allTextures;
	TTF playFieldTest = TTF(nullptr);
};