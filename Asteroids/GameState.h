#pragma once

#include <vector>

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;

	virtual bool HandleInput() = 0;
	virtual void HandleEvents() = 0;
	virtual void Render() = 0;
};

class GameState_MainMenu : public GameState
{
public:
	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();
private:

};

class GameState_PlayField : public GameState
{
public:
	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();
private:

};