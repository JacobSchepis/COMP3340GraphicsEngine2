#pragma once

#include <GL/glew.h>
#include <vector>

class TestCube {
public:
	TestCube(std::vector<float>& vertices, std::vector<GLuint>& indices);
	~TestCube();

	void setupVAO(GLuint sharedVBO, GLuint sharedEBO);

	GLuint VAO;

	std::vector<float> vertices;

	std::vector<GLuint> indices;
};