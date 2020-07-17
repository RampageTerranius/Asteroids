#include "main.h"
#include "timer.h"

int main(int argc, char* argv[])
{
	Timer timer;
	Timer fTimer;

	if (!game.HasActiveState())
	{
		debug.Log("Main", "main", "Game engine has no active state!");
		game.running = false;
	}

	fTimer.Start();

	Uint32 lastTickTime = fTimer.GetTicks();

	int framesCounted = 0;

	while (game.running)
	{
		timer.Start();
		
		game.HandleInput();
		game.HandleEvents();
		game.Render();		

		Uint32 timerFps = timer.GetTicks();
		framesCounted++;

		if (timerFps < 1000 / game.FRAME_RATE)
			SDL_Delay( (1000 / game.FRAME_RATE) - timerFps);

		if (fTimer.GetTicks() - lastTickTime >= 1000)
		{
			game.fps = framesCounted;
			framesCounted = 0;
			lastTickTime = fTimer.GetTicks();
		}
	}

	game.Cleanup();

	return 0;
}