#include "graphics/BufferObject.h"
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "entities/Entity.h"
#include "components/Transform.h"

#include <SDL/SDL.h>

#include <iostream>

#include "util/Debug.h"

BufferObject::BufferObject(std::vector<Mesh*> meshes) {
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    bufferData(meshes);

    GLuint vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    VAOMeshMap[vao] = meshes;
}

BufferObject::~BufferObject() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (auto& pair : VAOMeshMap) {
        GLuint vao = pair.first;
        std::vector<Mesh*>& meshes = pair.second;
        glDeleteVertexArrays(1, &vao);

        for (Mesh*& mesh : meshes)
            mesh = nullptr;
    }
}

void BufferObject::bufferData(std::vector<Mesh*>& meshes) {

    std::vector<float> vertexData;
    std::vector<GLuint> indexData;

    GLuint vertexOffset = 0;  // Track the total number of vertices processed
    GLuint indexOffset = 0;   // Track the total number of indices processed

    for (Mesh* mesh : meshes) {
        // Get vertices and indices from the mesh
        const auto vertices = mesh->getVertices();
        const auto indices = mesh->getIndices();

        // Add vertices to the vertex buffer
        if (!vertices.empty()) {
            vertexData.insert(vertexData.end(), vertices.begin(), vertices.end());
        }

        // Adjust the indices and add to the index buffer
        if (!indices.empty()) {
            // Offset the indices by the current number of vertices
            for (GLuint number : indices) {
                number += vertexOffset;  // Adjust indices based on the current vertex count
            }

            indexData.insert(indexData.end(), indices.begin(), indices.end());

            // Set the starting index for this mesh to the current index offset
            mesh->setStartingIndex(indexOffset);

            // Update the vertex and index offsets
            vertexOffset += vertices.size() / 3;  // Assuming each vertex has 3 components (x, y, z)
            indexOffset += indices.size();        // Update index offset based on number of indices added
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);
}

void BufferObject::draw(Shader* shader) const {
    for (const auto& pair : VAOMeshMap) {
        GLuint vao = pair.first;
        const std::vector<Mesh*>& meshes = pair.second;

        glBindVertexArray(vao);

        for (Mesh* mesh : meshes) {

            auto model = mesh->parent->getComponent<Transform>()->getModel();

            model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f, glm::vec3(0.5f, 1.0f, 0.0f));  // Rotate over time

            GLuint modelLoc = glGetUniformLocation(shader->Program, "model");

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            //std::cout << mesh->getIndices().size() << std::endl;

            glDrawElements(
                GL_TRIANGLES,
                mesh->getIndices().size(),
                GL_UNSIGNED_INT,
                (void*)(mesh->getStartingIndex() * sizeof(GLuint))
            );

            //Debug::checkOpenGLError("after draw");
        }
    }
    glBindVertexArray(0);
}