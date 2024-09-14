#include "components/Mesh.h"

#include <iostream>

Mesh::Mesh(Entity* parent,
	std::vector<float> vertices,
	std::vector<GLuint> indices
)	: IComponent(parent), m_startingIndex(0), vertices(vertices), indices(indices)
{
}

Mesh::~Mesh() {

}

std::vector<float> Mesh::getVertices() const {
	return vertices;
}
std::vector<GLuint> Mesh::getIndices() const {
	return indices;
}

GLuint Mesh::getStartingIndex() const {
	return m_startingIndex;
}

void Mesh::setStartingIndex(const GLuint startingIndex) {
	m_startingIndex += startingIndex;
}