#pragma once

#include <vector>
#include "EventHandle.h"
#include "Textures.h"
#include "TTF.h"

class GameState
{
	friend class GameEngine;
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	
	virtual bool HandleInput() = 0;
	virtual void HandleEvents() = 0;
	virtual void Render() = 0;

	void ClearCommands() { this->iManager->ClearInput(); }

protected:
	InputManager* iManager = new InputManager();	
};