#pragma once

#include <vector>
#include "EventHandle.h"
#include "Textures.h"
#include "TTF.h"

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
	GameState_MainMenu();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

private:
	InputManager* iManager = new InputManagerMainMenu();
	Textures allTextures;	
	TTF menuOptionStart;
	TTF menuOptionOptions;
	TTF menuOptionQuit;
};

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
	InputManager* iManager = new InputManagerPlayField();
	Textures allTextures;
};