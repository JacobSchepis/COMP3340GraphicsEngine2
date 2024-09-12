#include "graphics/Mesh.h"

Mesh::Mesh(std::vector<float>& vertices, std::vector<GLuint>& indices, MeshAttributeFlags& meshAttributes)
	: vertices(vertices), indices(indices), meshAttributes(meshAttributes), m_startingIndex(0)
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

MeshAttributeFlags Mesh::getMeshAttributes() const {
	return meshAttributes;
}

GLuint Mesh::getStartingIndex() const {
	return m_startingIndex;
}

void Mesh::setStartingIndex(const GLuint startingIndex) {
	m_startingIndex += startingIndex;
}