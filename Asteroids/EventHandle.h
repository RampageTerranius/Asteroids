#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include <vector>

#include "Textures.h"
#include "Command.h"

// With help from the following sources:
// https://gameprogrammingpatterns.com/command.html
// https://codereview.stackexchange.com/questions/55365/input-handling-system-using-the-command-pattern

enum KeyState
{
	KEYSTATE_RELEASED = false,
	KEYSTATE_PRESSED = true
};

class InputManager
{
public:
	~InputManager();
	InputManager();
	bool GenerateInput(std::vector<Command*>& commandVector);
	void Bind(int key, Command* command);
	SDL_Point GetMouseLocation();

protected:
	// Commands and keystates.
	std::map <int, Command*> commands;
	std::map <int, KeyState> state;
	std::map <int, KeyState> previousState;

	SDL_Point mouse;

	bool InputToActions();
	void DispatchCommands(std::vector<Command*>& command_queue);

	void OnKeyDownInput(SDL_Event& event);
	void OnKeyUpInput(SDL_Event& event);

	void OnMouseMotion(SDL_Event& event);
	void OnMouseDownInput(SDL_Event& event);
	void OnMouseUpInput(SDL_Event& event);

	bool IsHeld(int key);
};