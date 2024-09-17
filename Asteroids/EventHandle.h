#pragma once
#include <string>
#include <map>
#include <list>

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
	InputManager();
	bool GenerateInputAndDispatchCommands();
	bool GenerateInputAndDispatchCommands(std::list<Command*>& commandVector);
	bool GenerateInput();
	bool ProcessCommandList(Player* player);
	void Bind(int keycode, Command* command);
	SDL_Point GetMouseLocation();
	
	std::list<Command*> commandList;// list var type due to FILO operation types.
	void ClearInput();
	void ClearAll();

	// Functions for determining key presses.
	// These functions are used by DispatchCommands to determine if keys are in use and to run their corresponding commands if needed.
	// These functions can be called on their own should the programmer need to use a NON command based input system. 
	bool IsHeld(int key);
	bool JustPressed(int key);

protected:
	// Commands and keystates.
	// commands is a map built of the following:
	// An integer that represents the keycode for the command to activate.
	// The command to be activated.
	// for a full list of keycodes please see the following:
	// https://wiki.libsdl.org/SDL2/SDLKeycodeLookup
	std::map <int, Command*> commands;
	static std::map <int, KeyState> state;
	static std::map <int, bool> firstPress;

	SDL_Point mouse;	
	
	void DispatchCommands(std::list<Command*>& commandVector);

	void OnKeyDownInput(SDL_Event& event);
	void OnKeyUpInput(SDL_Event& event);

	void OnMouseMotion(SDL_Event& event);
	void OnMouseDownInput(SDL_Event& event);
	void OnMouseUpInput(SDL_Event& event);	
};