#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "components/IComponent.h"
#include "entities/Entity.h"


class Mesh : public IComponent{
public:
	Mesh(Entity* parent,
		std::vector<float> vertices,
		std::vector<GLuint> indices)

		: IComponent(parent), m_startingIndex(0), vertices(vertices), indices(indices)
	{
	}
	~Mesh() {

	}

	std::vector<float> getVertices() const {
		return vertices;
	}
	std::vector<GLuint> getIndices() const {
		return indices;
	}

	GLuint getStartingIndex() const {
		return m_startingIndex;
	}

	void setStartingIndex(const GLuint startingIndex) {
		m_startingIndex += startingIndex;
	}

private:
	GLuint m_startingIndex;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
};