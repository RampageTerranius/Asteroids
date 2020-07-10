#pragma once
#include <SDL.h>

class Timer
{
public:
    Timer();

    // The various clock actions.
    void Start();
    void Stop();
    void Pause();
    void Unpause();

    // Gets the timer's time.
    Uint32 GetTicks();

    // Checks the status of the timer.
    bool IsStarted();
    bool IsPaused();

private:
    // The clock time when the timer started.
    Uint32 startTicks;

    // The ticks stored when the timer was paused.
    Uint32 pausedTicks;

    // The timer status.
    bool paused;
    bool started;
};