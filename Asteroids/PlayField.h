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

		player->velX -= (cos((player->rotation + 90.0f) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		player->velY -= (sin((player->rotation + 90.0f) * (M_PI / 180)) * 0.5f * player->velocity) * i;

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

		player->velX += (cos((player->rotation + 90.0f) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		player->velY += (sin((player->rotation + 90.0f) * (M_PI / 180)) * 0.5f * player->velocity) * i;
		return true;
	}
};

class CommandFire : public Command
{
public:
	CommandFire() { this->allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		return true;
	}
};

class CommandBoost : public Command
{
public:
	CommandBoost() { this->allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->speedBoost = true;
		return true;
	}
};

class CommandEqualizeVelocity : public Command
{
public:
	CommandEqualizeVelocity() { this->allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		if (player->velX > 0)
		{
			player->velX -= (player->velocity / 4);
			if (player->velX < 0)
				player->velX = 0;
		}
		else if (player->velX < 0)
		{
			player->velX += (player->velocity / 4);
			if (player->velX > 0)
				player->velX = 0;
		}

		if (player->velY > 0)
		{
			player->velY -= (player->velocity / 4);
			if (player->velY < 0)
				player->velY = 0;
		}
		else if (player->velY < 0)
		{
			player->velY += (player->velocity / 4);
			if (player->velY > 0)
				player->velY = 0;
		}
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

	Command* commandFire = new CommandFire();
	Command* commandForwards = new CommandMoveForward();
	Command* commandBackwards = new CommandMoveBackwards();
	Command* commandRotateLeft = new CommandMoveLeft();
	Command* commandRotateRight = new CommandMoveRight();
	Command* commandBoost = new CommandBoost();
	Command* commandEqualizeVelocity = new CommandEqualizeVelocity();
};