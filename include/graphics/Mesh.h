#pragma once

#include <GL/glew.h>
#include <vector>

class Mesh {
public:
	Mesh();
	~Mesh();

	void bindVAO(const GLuint& newVAO);
	GLuint getVAO();

	std::vector<float> getVertices();
	std::vector<GLuint> getIndices();

private:

	GLuint m_VAO;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
};