#pragma once

#include <vector>
#include "graphics/TestCube.h"
#include "shaders/Shader.h"
#include <SDL/SDL.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	void sendObjectsToBuffer(std::vector<TestCube>& testCubes);
	void sendObjectsToBuffer(TestCube& testCube);

	void render(Shader* shader, SDL_Window* window);

private:
	GLuint m_VBO, m_EBO;
};