#pragma once
#include "Entity.h"

class Command
{
public:
	virtual bool Execute(Player* player) { return true; }
	bool allowContinuousExecution = true;
};