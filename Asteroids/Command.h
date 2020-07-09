#pragma once
#include "Entity.h"

class Command
{
public:
	virtual bool Execute(Player* player) = 0;
	bool allowContinuousExecution = true;
};