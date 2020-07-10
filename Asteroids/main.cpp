#include "main.h"
#include "timer.h"

int main(int argc, char* argv[])
{
	game.Init();

	Timer timer;

	timer.Start();

	while (game.running)
	{
		timer.Start();

		if (game.HasActiveState())
		{
			game.HandleInput();
			game.HandleEvents();
			game.Render();
		}
		else
		{
			debug.Log("Main", "main", "Game engine has no active state!");
			return 1;
		}
	}

	game.Cleanup();

	return 0;
}