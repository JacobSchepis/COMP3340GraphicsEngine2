#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "components/abstract/IComponent.hpp"
#include <graphics/Vertex.hpp>


struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint VAO, VBO, EBO;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
        : vertices(vertices), indices(indices), VAO(0), VBO(0), EBO(0)
	{
        setupMesh();
    }

    Mesh() {}

	~Mesh() {}

    void setupMesh() {
        // Generate VAO, VBO, and EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind VAO
        glBindVertexArray(VAO);

        // Bind and fill VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Bind and fill EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        // Unbind VAO
        glBindVertexArray(0);
    }
};