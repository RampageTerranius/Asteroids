#include "main.h"
#include "Debug.h"
#include "timer.h"

int main(int argc, char* argv[])
{
	Timer timer;
	Timer fTimer;

	// Check if the game has an active state at all.
	if (!game.HasActiveState())
	{
		debug.Log("Main", "main", "Game engine has no active state!");
		game.running = false;
	}

	// Start a timer for monitoring framerates and regulatign them.
	fTimer.Start();
	Uint32 lastTickTime = fTimer.GetTicks();
	int framesCounted = 0;

	// Standard game loop.
	while (game.running)
	{
		timer.Start();
		
		// Ask the game engine to handle all input/events and rendering.
		game.HandleInput();
		game.HandleEvents();
		game.Render();		

		Uint32 timerFps = timer.GetTicks();
		framesCounted++;

		if (timerFps < (1000u / game.FRAME_RATE))
		{
			SDL_Delay((1000u / game.FRAME_RATE) - timerFps);
		}

		if (fTimer.GetTicks() - lastTickTime >= 1000)
		{
			game.fps = framesCounted;
			framesCounted = 0;
			lastTickTime = fTimer.GetTicks();
		}
	}

	// Ask the game engine to cleanup before closing the program.
	game.Cleanup();
	return 0;
}