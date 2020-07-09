#include "EventHandle.h"
#include "Debug.h"

#include <SDL.h>

std::map <int, KeyState> InputManager::state = std::map <int, KeyState>();
std::map <int, bool> InputManager::firstPress = std::map <int, bool>();

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

// Generates all input and processes commands.
// Used if Commands are required.
bool InputManager::GenerateInputAndDispatchCommands(std::vector<Command*>& CommandVector)
{
	if (!GenerateInput())
		return false;
	else
	{
		DispatchCommands(CommandVector);
		return true;
	}
}

bool InputManager::GenerateInputAndDispatchCommands()
{
	return GenerateInputAndDispatchCommands(this->commandList);
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
void InputManager::DispatchCommands(std::vector<Command*>& commandVector)
{
	for (std::map<int, Command*>::iterator iter = commands.begin(); iter != commands.end(); iter++)
	{
		if (iter->second->allowContinuousExecution)
		{
			if (this->IsHeld(iter->first))
				commandVector.push_back(iter->second);
		}
		else
			if (this->JustPressed(iter->first))
				commandVector.push_back(iter->second);
	}
}

// Assigns the given command to the given key.
void InputManager::Bind(int key, Command* command)
{
	commands[key] = command;
}

// Functions used as part of InputToActions()
void InputManager::OnMouseMotion(SDL_Event& event)
{
	SDL_GetMouseState(&mouse.x, &mouse.y);
}

void InputManager::OnKeyDownInput(SDL_Event& event)
{
	state[event.key.keysym.sym] = KeyState::pressed;
}

void InputManager::OnKeyUpInput(SDL_Event& event)
{
	firstPress[event.key.keysym.sym] = false;
	state[event.key.keysym.sym] = KeyState::released;
}

void InputManager::OnMouseDownInput(SDL_Event& event)
{
	state[event.button.button] = KeyState::pressed;
}

void InputManager::OnMouseUpInput(SDL_Event& event)
{
	firstPress[event.button.button] = false;
	state[event.button.button] = KeyState::released;
}

// Checks if the key is currently being held.
bool InputManager::IsHeld(int key)
{
	if (state[key] == KeyState::pressed)
		return true;
	else
		return false;
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
		return false;
}

void InputManager::ClearInput()
{
	this->commandList.clear();

	for (std::map<int, KeyState>::iterator iter = state.begin(); iter != state.end(); iter++)
		iter->second = KeyState::released;

	for (std::map<int, bool>::iterator iter = firstPress.begin(); iter != firstPress.end(); iter++)
		iter->second = false;
}

SDL_Point InputManager::GetMouseLocation()
{
	return SDL_Point{ mouse.x, mouse.y };
}