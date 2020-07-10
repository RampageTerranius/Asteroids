#pragma once
#include "GameState.h"
#include "Entity.h"
#include "Command.h"

class CommandMoveLeft : public Command
{
public:
	CommandMoveLeft() { this->allowContinuousExecution = true; }
	bool Execute(Player* player) { player->rotation -= player->turnRate; return true; }
};

class CommandMoveRight : public Command
{
public:
	CommandMoveRight() { this->allowContinuousExecution = true; }
	bool Execute(Player* player) { player->rotation += player->turnRate; return true; }
};

class CommandMoveForward : public Command
{
public:
	CommandMoveForward() { this->allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->velX -= (cos((player->rotation + 90) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		player->velY -= (sin((player->rotation + 90) * (M_PI / 180)) * 0.5f * player->velocity) * i;

		return true;
	}
};

class CommandMoveBackwards : public Command
{
public:
	CommandMoveBackwards() { this->allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->velX += (cos((player->rotation + 90) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		player->velY += (sin((player->rotation + 90) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		return true;
	}
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

	void CalcPlayerMovement();

private:
	Textures allTextures;
	Player player;
	Bullets bullets;
};