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

// used to determine the current state of a key/button.
enum class KeyState
{
	released = false,
	pressed = true
};

// Handles input.
// Currently designed for use primarily with mouse/keyboard.
class InputManager
{
public:
	~InputManager();
	InputManager();
	bool GenerateInputAndDispatchCommands();
	bool GenerateInputAndDispatchCommands(std::vector<Command*>& commandVector);
	bool GenerateInput();
	void Bind(int key, Command* command);
	SDL_Point GetMouseLocation();
	
	std::vector<Command*> commandList;
	void ClearInput();

	// Functions for determining key presses.
	// These functions are used by DispatchCommands to determine if keys are in use and to run their corresponding commands if needed.
	// These functions can be called on their own should the programmer need to use a NON command based input system (). 
	bool IsHeld(int key);
	bool JustPressed(int key);

protected:
	// Commands and keystates.
	std::map <int, Command*> commands;
	static std::map <int, KeyState> state;
	static std::map <int, bool> firstPress;

	SDL_Point mouse;	
	
	void DispatchCommands(std::vector<Command*>& commandVector);

	void OnKeyDownInput(SDL_Event& event);
	void OnKeyUpInput(SDL_Event& event);

	void OnMouseMotion(SDL_Event& event);
	void OnMouseDownInput(SDL_Event& event);
	void OnMouseUpInput(SDL_Event& event);	
};