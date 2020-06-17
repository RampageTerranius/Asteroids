#include "main.h"

int main(int argc, char* argv[])
{
	game.Init();

	while (game.Running())
	{
		game.HandleInput();
		//game.HandleEvents();
		//game.Render();
	}

	game.Cleanup();

	return 0;
}