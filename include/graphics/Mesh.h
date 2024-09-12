#pragma once

#include <GL/glew.h>
#include <vector>

#include "graphics/MeshAttributeFlags.h"

class Mesh {
public:
	Mesh(std::vector<float>& vertices, std::vector<GLuint>& indices, MeshAttributeFlags& meshAttributes);
	~Mesh();

	std::vector<float> getVertices() const;
	std::vector<GLuint> getIndices() const;

	MeshAttributeFlags getMeshAttributes() const;

	GLuint getStartingIndex() const;
	void setStartingIndex(const GLuint startingIndex);

	bool hasPosition() const { return hasFlag(meshAttributes, MeshAttributeFlags::Position); }
	bool hasNormal() const { return hasFlag(meshAttributes, MeshAttributeFlags::Normal); }
	bool hasColor() const { return hasFlag(meshAttributes, MeshAttributeFlags::Color); }
	bool hasTexCoord() const { return hasFlag(meshAttributes, MeshAttributeFlags::TexCoord); }

	

private:
	MeshAttributeFlags meshAttributes;

	GLuint m_startingIndex;

	std::vector<float> vertices;
	std::vector<GLuint> indices;
};