#pragma once
#include "Entity.h"

// Base command class.
// Used by each state when creating their own commands.
class Command
{
public:
	virtual bool Execute(Player* player) { return true; }
	bool allowContinuousExecution = true;
};