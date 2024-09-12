#pragma once

#include "SDL/SDL.h"
#include <glm/glm.hpp>

class InputManager
{
public:

	InputManager(const InputManager&) = delete;

	static InputManager& Instance();

	void ProcessInputs();

	bool GetApplicationQuit();

	SDL_MouseMotionEvent* GetMouseEvent();

	glm::vec2* GetwasdInputVector();

private:
	InputManager();

	SDL_MouseMotionEvent mouseEvent;

	bool applicationQuit;

	glm::vec2 wasdInputVector;

	void ProcessKeyDownEvent(SDL_Event keysym);

	void ResetInputValues();

};