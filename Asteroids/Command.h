#pragma once

class Command
{
public:
	virtual bool Execute() = 0;
	bool allowContinuousExecution = true;
};