#include "main.h"
#include "timer.h"

int main(int argc, char* argv[])
{
	game.Init();

	Timer timer;

	if (!game.HasActiveState())
	{
		debug.Log("Main", "main", "Game engine has no active state!");
		game.running = false;
	}

	while (game.running)
	{
		timer.Start();
		
		game.HandleInput();
		game.HandleEvents();
		game.Render();

		float timerFps = timer.GetTicks();

		if (timerFps < 1000 / game.FRAME_RATE)
			SDL_Delay( (1000 / game.FRAME_RATE) - timerFps);
	}

	game.Cleanup();

	return 0;
}