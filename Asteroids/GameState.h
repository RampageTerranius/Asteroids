#pragma once
#include "EventHandle.h"
#include "Textures.h"
#include "Audio.h"

// Base game state.
// Functions must be provided by child classes.
// GameStates are automatically called upon while at the end of the vector.
class GameState
{
public:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	
	virtual bool HandleInput() = 0;
	virtual void HandleEvents() = 0;
	virtual void Render() = 0;

	Textures allTextures;
	Sounds allSounds;
	InputManager iManager;
};