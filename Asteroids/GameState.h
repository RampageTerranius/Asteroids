#pragma once

#include <vector>

class GameState
{
public:
	void Init();
	void Cleanup();

	void Pause();
	void Resume();

	void HandleInput();
	void HandleEvents();
	void Render();
};