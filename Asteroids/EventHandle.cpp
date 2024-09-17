#include "EventHandle.h"

std::map <int, KeyState> InputManager::state = std::map <int, KeyState>();
std::map <int, bool> InputManager::firstPress = std::map <int, bool>();

// Constructor.
InputManager::InputManager()
{
	mouse = SDL_Point{ 0,0 };	
}

// Clear the command list.
void InputManager::ClearAll()
{
	ClearInput();
	commandList.clear();
	commands.clear();
	state.clear();
	firstPress.clear();	
}

// Generates all input and processes commands.
// Use this if you wish to give a custom list of commands to be processed.
// (for example you wish to force a quit programatically)
bool InputManager::GenerateInputAndDispatchCommands(std::list<Command*>& CommandVector)
{
	// First poll all events reported by SDL.
	if (!GenerateInput())
	{
		return false;
	}
	else
	{
		// IF no events asked to quit then go ahead and process the input and dispatch commands to the list as needed.
		DispatchCommands(CommandVector);
		return true;
	}
}

// Automated version of above using internal command list.

bool InputManager::GenerateInputAndDispatchCommands()
{
	return GenerateInputAndDispatchCommands(commandList);
}

// Gather the users input and maps it as required.
// Used to process only input.
// This can be called alone to ONLY process input if need be.
bool InputManager::GenerateInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type)
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:			
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				return false;
			}

			OnKeyDownInput(event);
			break;

		case SDL_KEYUP:
			OnKeyUpInput(event);
			break;

		case SDL_MOUSEMOTION:
			OnMouseMotion(event);
			break;

		case SDL_MOUSEBUTTONDOWN:
			OnMouseDownInput(event);
			break;

		case SDL_MOUSEBUTTONUP:
			OnMouseUpInput(event);
			break;
		}
	return true;
}

// Add all commands to a list for processing.
void InputManager::DispatchCommands(std::list<Command*>& commandVector)
{
	// Iterate over all commands and add them to the commandVector as needed.
	for (std::map<int, Command*>::iterator it = commands.begin(); it != commands.end(); it++)
	{		
		if (it->second->allowContinuousExecution)
		{
			// If the command is allowed to be continuous and is being held down then add it to the list of commands to process.
			if (IsHeld(it->first))
			{
				commandVector.push_back(it->second);
			}
		}
		else if (JustPressed(it->first))
		{
			commandVector.push_back(it->second);
		}
	}
}

// Loops through all currently qued commands and executes them.
// Relys on DispatchCommands to have been run beforehand.
bool InputManager::ProcessCommandList(Player* player)
{
	while (!commandList.empty())
	{
		// While there are still commands in the list,
		// process each one, one by one using the FILO principle.
		if (!commandList.back()->Execute(player))
		{
			return false;
			break;
		}

		if (commandList.size() > 0)
		{
			commandList.pop_back();
		}
	}

	return true;
}

// Assigns the given command to the given keycode.
// keycode is the decimal value of the button being pressed (in SDL_keycode)
// Command is a pointer to a given command.
// For a list of keycodes please see the following:
// https://wiki.libsdl.org/SDL2/SDLKeycodeLookup
void InputManager::Bind(int keycode, Command* command)
{
	commands[keycode] = command;
}

// Update stored mouse location. 
void InputManager::OnMouseMotion(SDL_Event& event)
{
	SDL_GetMouseState(&mouse.x, &mouse.y);
}

// Update current state of a key to pressed.
void InputManager::OnKeyDownInput(SDL_Event& event)
{
	state[event.key.keysym.sym] = KeyState::pressed;
}

// Update current state of a key to released.
void InputManager::OnKeyUpInput(SDL_Event& event)
{
	firstPress[event.key.keysym.sym] = false;
	state[event.key.keysym.sym] = KeyState::released;
}

// Update current state of mouse button to pressed.
void InputManager::OnMouseDownInput(SDL_Event& event)
{
	state[event.button.button] = KeyState::pressed;
}

// Update current state of mouse button to released.
void InputManager::OnMouseUpInput(SDL_Event& event)
{
	firstPress[event.button.button] = false;
	state[event.button.button] = KeyState::released;
}

// Checks if the key is currently being held.
// Held means it has been reported as pressed down for 2 or more input cycles.
bool InputManager::IsHeld(int key)
{
	if (state[key] == KeyState::pressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Checks if the key is currently in a pushed state then triggers a flag to not show it again untill key is let up.
bool InputManager::JustPressed(int key)
{
	if (state[key] == KeyState::pressed && firstPress[key] == false)
	{
		firstPress[key] = true;
		return true;
	}
	else
	{
		return false;
	}
}

// Clear all commands from the list and reset all keys to their default state.
// Should only be used for cleanup operations.
void InputManager::ClearInput()
{
	commandList.clear();

	for (std::map<int, KeyState>::iterator iter = state.begin(); iter != state.end(); iter++)
	{
		iter->second = KeyState::released;
	}

	for (std::map<int, bool>::iterator iter = firstPress.begin(); iter != firstPress.end(); iter++)
	{
		iter->second = false;
	}
}

// Return the currently reported location of the mouse.
SDL_Point InputManager::GetMouseLocation()
{
	return SDL_Point{ mouse.x, mouse.y };
}