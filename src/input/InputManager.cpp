#include "input/InputManager.h"

InputManager& InputManager::Instance()
{
	static InputManager inputManager;
	return inputManager;
}

void InputManager::ProcessInputs()
{
	SDL_Event evnt;

	ResetInputValues();

	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			applicationQuit = true;
			break;

		case SDL_MOUSEMOTION:
			mouseEvent = evnt.motion;
			break;
		case SDL_KEYDOWN:
			ProcessKeyDownEvent(evnt);

		}
	}
}

bool InputManager::GetApplicationQuit()
{
	return applicationQuit;
}

SDL_MouseMotionEvent* InputManager::GetMouseEvent()
{
	return &mouseEvent;
}

glm::vec2* InputManager::GetwasdInputVector()
{
	return &wasdInputVector;
}

//private instance methods

InputManager::InputManager() : mouseEvent(), applicationQuit(false), wasdInputVector(glm::vec2(0.0f))
{

}

void InputManager::ProcessKeyDownEvent(SDL_Event evnt)
{
	switch (evnt.key.keysym.sym)
	{
	case SDLK_UP:
		wasdInputVector.y += 1.0f;
		break;
	case SDLK_DOWN:
		wasdInputVector.y -= 1.0f;
		break;
	case SDLK_RIGHT:
		wasdInputVector.x += 1.0f;
		break;
	case SDLK_LEFT:
		wasdInputVector.x -= 1.0f;
		break;
	}
}

void InputManager::ResetInputValues()
{
	wasdInputVector.x = 0.0f;
	wasdInputVector.y = 0.0f;
}
