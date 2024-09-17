#pragma once
#include "Command.h"
#include "Entity.h"

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

// Command for when a user has pressed forwards key.
class CommandMoveForward : public Command
{
public:
	// Constructor, allow continuous execution of this command.
	CommandMoveForward() { allowContinuousExecution = true; }

	// Executes the command for the player.
	bool Execute(Player* player)
	{
		double i = 1;
		// If player is speed boosting then double the effect.
		if (player->speedBoost)
		{
			i = 2;
		}

		// Calculate the velocity of the player.
		player->velX -= (static_cast <float> (cos((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);
		player->velY -= (static_cast <float> (sin((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);

		if (!player->speedBoost)
		{
			player->moveForwardsSound->Play();
		}
		else
		{
			player->moveForwardsBoostSound->Play();
		}

		return true;
	}
};

// Command for when a user has pressed backwards key.
class CommandMoveBackwards : public Command
{
public:
	// Constructor, allow continuous execution of this command.
	CommandMoveBackwards() { allowContinuousExecution = true; }

	// Executes the command for the player.
	bool Execute(Player* player)
	{
		double i = 1;
		// If player is speed boosting then double the effect.
		if (player->speedBoost)
		{
			i = 2;
		}

		// Calculate the velocity of the player.
		player->velX += (static_cast <float> (cos((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);
		player->velY += (static_cast <float> (sin((player->rotation + 90.0f) * (M_PI / 180.0f)) * player->velocity)) * static_cast <float> (i);

		if (!player->speedBoost)
		{
			player->moveBackwardsSound->Play();
		}
		else
		{
			player->moveBackwardsBoostSound->Play();
		}

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
		bool hasEqualized = false;

		if (player->velX > 0)
		{
			player->velX -= (player->velocity / 2);
			if (player->velX < 0)
				player->velX = 0;
			hasEqualized = true;
		}
		else if (player->velX < 0)
		{
			player->velX += (player->velocity / 2);
			if (player->velX > 0)
				player->velX = 0;
			hasEqualized = true;
		}

		if (player->velY > 0)
		{
			player->velY -= (player->velocity / 2);
			if (player->velY < 0)
				player->velY = 0;
			hasEqualized = true;
		}
		else if (player->velY < 0)
		{
			player->velY += (player->velocity / 2);
			if (player->velY > 0)
				player->velY = 0;
			hasEqualized = true;
		}

		if (hasEqualized = true)
			player->equalizeSound->Play();

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