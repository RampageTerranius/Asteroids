#pragma once
#include "GameState.h"
#include "Command.h"
#include "Command_PlayField.h"
#include "TTF.h"

// Gamestate for the primary play field.
// The game itself is played in this game state
class GameState_PlayField : public GameState
{
public:
	GameState_PlayField();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

	int asteroidAutoSpawnTimer;

	int currentScore;

private:
	Player player;
	TTFs allTTF;

	void CheckForCollisons();
	void CheckForNewAsteroids();

	Texture* background;

	TTF* fps;
	TTF* score;

	// List of commands for user.
	Command* commandFire = new CommandFire();
	Command* commandForwards = new CommandMoveForward();
	Command* commandBackwards = new CommandMoveBackwards();
	Command* commandRotateLeft = new CommandMoveLeft();
	Command* commandRotateRight = new CommandMoveRight();
	Command* commandBoost = new CommandBoost();
	Command* commandEqualizeVelocity = new CommandEqualizeVelocity();
	Command* commandCreateAsteroid = new CommandCreateAsteroid();
};