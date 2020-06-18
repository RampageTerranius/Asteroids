#include "GameState.h"
#include "EventHandle.h"

void GameState_PlayField::Init()
{

}

void GameState_PlayField::Cleanup()
{

}

bool GameState_PlayField::HandleInput()
{
	std::vector<Command*> CommandList;

	bool quitProgram = iManager.GenerateInput(CommandList);

	while (!CommandList.empty())
	{
		//CommandList.back()->Execute(currentPlayer);
		CommandList.pop_back();
	}

	return quitProgram;
}

void GameState_PlayField::HandleEvents()
{

}

void GameState_PlayField::Render()
{

}