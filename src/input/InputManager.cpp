#include "input/InputManager.h"

#include <iostream>

InputManager& InputManager::Instance()
{
	static InputManager inputManager;
	return inputManager;
}

void InputManager::processInputs()
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

	int deltaX, deltaY;
	SDL_GetRelativeMouseState(&deltaX, &deltaY);
	mouseDelta = glm::vec2(deltaX, deltaY);
}

bool InputManager::getApplicationQuit()
{
	return applicationQuit;
}

SDL_MouseMotionEvent* InputManager::getMouseEvent()
{
	return &mouseEvent;
}

glm::vec2& InputManager::getMouseDelta() {
	return mouseDelta;
}

glm::vec2& InputManager::getWasdInputVector()
{
	return wasdInputVector;
}

int InputManager::getVerticalInput() {
	return verticalInput;
}

//private instance methods

InputManager::InputManager() : 
	mouseEvent(), applicationQuit(false), wasdInputVector(glm::vec2(0.0f)), mouseDelta(glm::vec2(0.0f)), verticalInput(0)
{

}

void InputManager::ProcessKeyDownEvent(SDL_Event evnt)
{
	switch (evnt.key.keysym.sym)
	{
	case SDLK_w:
		wasdInputVector.y += 1.0f;
		break;
	case SDLK_s:
		wasdInputVector.y -= 1.0f;
		break;
	case SDLK_d:
		wasdInputVector.x += 1.0f;
		break;
	case SDLK_a:
		wasdInputVector.x -= 1.0f;
		break;

	case SDLK_SPACE:
		verticalInput += 1;
		break;

	case SDLK_LCTRL:
		verticalInput -= 1;
		break;
	}

	
}

void InputManager::ResetInputValues()
{
	wasdInputVector = glm::vec2(0.0f);
	mouseDelta = glm::vec2(0.0f);
	verticalInput = 0;
}
