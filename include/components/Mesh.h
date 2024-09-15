#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "IComponent.h"

class Mesh : public IComponent{
public:
	Mesh(std::vector<float> vertices, std::vector<GLuint> indices);

	~Mesh();

	std::vector<float> getVertices() const;
	std::vector<GLuint> getIndices() const;

	GLuint getStartingIndex() const;

	void setStartingIndex(const GLuint startingIndex);

private:
	GLuint m_startingIndex;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
};