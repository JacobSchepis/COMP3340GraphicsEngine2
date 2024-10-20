#pragma once

#include "SDL/SDL.h"
#include <glm/glm.hpp>

class InputManager
{
public:

	InputManager(const InputManager&) = delete;

	static InputManager& Instance();

	void processInputs();

	bool getApplicationQuit();

	SDL_MouseMotionEvent* getMouseEvent();

	glm::vec2& getMouseDelta();

	glm::vec2& getWasdInputVector();

	int getVerticalInput();

	bool getPressedEscape();

private:
	InputManager();

	SDL_MouseMotionEvent mouseEvent;

	bool applicationQuit;

	glm::vec2 wasdInputVector;
	glm::vec2 mouseDelta;
	int verticalInput;

	void ProcessKeyDownEvent(SDL_Event keysym);

	void ResetInputValues();

	bool pressedEscape;
};