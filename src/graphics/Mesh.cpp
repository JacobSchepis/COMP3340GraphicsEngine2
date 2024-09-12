#include "graphics/Mesh.h"
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(std::vector<float> vertices, 
			std::vector<GLuint> indices, 
			glm::vec3 position,
			glm::vec3 rotation, 
			glm::vec3 scale)

	: vertices(vertices), indices(indices), meshAttributes(meshAttributes), m_startingIndex(0),
		position(position), rotation(rotation), scale(scale), model(glm::mat4(1.0f))
{
	updateModelMatrix();
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

void Mesh::updateModelMatrix() {

    // Apply transformations in order: Scale -> Rotate -> Translate
    model = glm::translate(model, position);  // Translate to position
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate around X-axis
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate around Y-axis
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate around Z-axis
    model = glm::scale(model, scale);  // Apply scaling
}

glm::mat4 Mesh::getModel() {
	return model;
}