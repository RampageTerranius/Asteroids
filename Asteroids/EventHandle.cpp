#include "EventHandle.h"
#include "Debug.h"

#include <SDL.h>

InputManager::InputManager()
{
	mouse = SDL_Point{ 0,0 };
}

// Delete all command pointers on deconstruction.
InputManager::~InputManager()
{
	for (std::map<int, Command*>::iterator iter = commands.begin(); iter != commands.end(); iter++)
		delete iter->second;
}

SDL_Point InputManager::GetMouseLocation()
{
	return SDL_Point{ mouse.x, mouse.y };
}

// Gather the users input and maps it as required.
bool InputManager::InputToActions()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		switch (event.type)
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return false;
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
void InputManager::DispatchCommands(std::vector<Command*>& command_queue)
{
	for (std::map<int, Command*>::iterator iter = commands.begin(); iter != commands.end(); iter++)
		if (IsHeld(iter->first))
			command_queue.push_back(iter->second);
}

bool InputManager::GenerateInput(std::vector<Command*>& CommandVector)
{
	if (!InputToActions())
		return false;
	else
	{
		DispatchCommands(CommandVector);
		return true;
	}
}

void InputManager::OnMouseMotion(SDL_Event& event)
{
	SDL_GetMouseState(&mouse.x, &mouse.y);
}

void InputManager::OnKeyDownInput(SDL_Event& event)
{
	state[event.key.keysym.sym] = KEYSTATE_PRESSED;
}

void InputManager::OnKeyUpInput(SDL_Event& event)
{
	state[event.key.keysym.sym] = KEYSTATE_RELEASED;
}

void InputManager::OnMouseDownInput(SDL_Event& event)
{
	state[event.button.button] = KEYSTATE_PRESSED;
}

void InputManager::OnMouseUpInput(SDL_Event& event)
{
	state[event.button.button] = KEYSTATE_RELEASED;
}

// Check if the key is currently being held.
bool InputManager::IsHeld(int key)
{
	return state[key];
}

void InputManager::Bind(int key, Command* command)
{
	commands[key] = command;
}

void InputManager::Bind(int key, std::string str)
{
	Command* command = nullptr;

	// TODO: once again... there has to be a better way to do this... switches of type string dont work but surely there is a better way to go about this then a nest of if/else
	/*if (str == "MoveUp")
		command = moveUp;
	else if (str == "MoveDown")
		command = moveDown;
	else if (str == "MoveLeft")
		command = moveLeft;
	else if (str == "MoveRight")
		command = moveRight;
	else if (str == "Fire")
		command = fire;
	else if (str == "Reload")
		command = reload;
	else if (str == "Dodge")
		command = dodge;
	else if (str == "Build")
		command = build;
	else if (str == "NextWeapon")
		command = changeWeaponNext;
	else if (str == "LastWeapon")
		command = changeWeaponLast;*/

	if (command != nullptr)
		Bind(key, command);
	else
		debug.Log("InputManager", "Bind", "Failed to find a command that '" + str + "' could be bound to");
}

InputManager iManager = InputManager();