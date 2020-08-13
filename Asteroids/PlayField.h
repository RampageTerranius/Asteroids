#pragma once
#include "GameState.h"
#include "Entity.h"
#include "Command.h"

class CommandMoveLeft : public Command
{
public:
	CommandMoveLeft() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->rotation -= player->turnRate * static_cast <float> (i);
		return true;
	}
};

class CommandMoveRight : public Command
{
public:
	CommandMoveRight() { allowContinuousExecution = true; }
	bool Execute(Player* player) 
	{ 
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->rotation += player->turnRate * static_cast <float> (i);
		return true;
	}
};

class CommandMoveForward : public Command
{
public:
	CommandMoveForward() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->velX -= ( static_cast <float> (cos((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);
		player->velY -= ( static_cast <float> (sin((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);

		return true;
	}
};

class CommandMoveBackwards : public Command
{
public:
	CommandMoveBackwards() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		double i = 1;
		if (player->speedBoost)
			i = 2;

		player->velX += (static_cast <float> (cos((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);
		player->velY += (static_cast <float> (sin((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);
		return true;
	}
};

class CommandFire : public Command
{
public:
	CommandFire() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->FireWeapon();		
		return true;
	}
};

class CommandBoost : public Command
{
public:
	CommandBoost() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->speedBoost = true;
		return true;
	}
};

class CommandEqualizeVelocity : public Command
{
public:
	CommandEqualizeVelocity() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{		
		if (player->velX > 0)
		{
			player->velX -= (player->velocity / 2);
			if (player->velX < 0)
				player->velX = 0;
		}
		else if (player->velX < 0)
		{
			player->velX += (player->velocity / 2);
			if (player->velX > 0)
				player->velX = 0;
		}

		if (player->velY > 0)
		{
			player->velY -= (player->velocity / 2);
			if (player->velY < 0)
				player->velY = 0;
		}
		else if (player->velY < 0)
		{
			player->velY += (player->velocity / 2);
			if (player->velY > 0)
				player->velY = 0;
		}
		return true;
	}
};

class CommandCreateAsteroid : public Command
{
public:
	CommandCreateAsteroid() { allowContinuousExecution = false; }
	bool Execute(Player* player)
	{
		allAsteroids.CreateAsteroid(player);
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

	int asteroidAutoSpawnTimer;

private:
	Player player;
	TTFs allTTF;

	void CheckForCollisons();
	void CheckForNewAsteroids();

	Texture* background;

	TTF* fps;

	Command* commandFire = new CommandFire();
	Command* commandForwards = new CommandMoveForward();
	Command* commandBackwards = new CommandMoveBackwards();
	Command* commandRotateLeft = new CommandMoveLeft();
	Command* commandRotateRight = new CommandMoveRight();
	Command* commandBoost = new CommandBoost();
	Command* commandEqualizeVelocity = new CommandEqualizeVelocity();
	Command* commandCreateAsteroid = new CommandCreateAsteroid();
};