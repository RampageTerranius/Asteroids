#pragma once
#include "GameState.h"
#include "Entity.h"

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
	Player player;
	Bullets bullets;
};