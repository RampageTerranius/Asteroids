#include "GameState.h"
#include "EventHandle.h"


void GameState_MainMenu::Init()
{

}

void GameState_MainMenu::Cleanup()
{

}

bool GameState_MainMenu::HandleInput()
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

void GameState_MainMenu::HandleEvents()
{

}

void GameState_MainMenu::Render()
{

}
