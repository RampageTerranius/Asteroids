#include "Command.h"
#include "GameEngine.h"
#include "PlayField.h"

bool CommandMainMenuSelect::Execute()
{
	switch (game.menuOption)
	{
		case 1:
			break;

		case 2:
			return false;
			break;

		case 3:			
 			game.PushNewState(new GameState_PlayField());
			break;
	}

	return true;
};

bool CommandPlayFieldReturnToLastState::Execute()
{
	return false;
}