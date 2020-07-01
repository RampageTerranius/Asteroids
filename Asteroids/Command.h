#pragma once

class Command
{
public:
	virtual bool Execute() = 0;
};

class CommandMainMenuSelect : public Command
{
	bool Execute();
};

class CommandPlayFieldReturnToLastState : public Command
{
	bool Execute();
};