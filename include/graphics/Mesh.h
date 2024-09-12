#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

#include "graphics/MeshAttributeFlags.h"

class Mesh {
public:
	Mesh(std::vector<float> vertices, 
			std::vector<GLuint> indices, 
			glm::vec3 position,
			glm::vec3 rotation,
			glm::vec3 scale);
	~Mesh();

	std::vector<float> getVertices() const;
	std::vector<GLuint> getIndices() const;

	MeshAttributeFlags getMeshAttributes() const;

	GLuint getStartingIndex() const;
	void setStartingIndex(const GLuint startingIndex);

	glm::mat4 getModel();

private:
	MeshAttributeFlags meshAttributes;

	GLuint m_startingIndex;

	std::vector<float> vertices;
	std::vector<GLuint> indices;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 model;

	void updateModelMatrix();
};