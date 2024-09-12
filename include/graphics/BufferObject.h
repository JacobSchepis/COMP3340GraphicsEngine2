#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include "graphics/Mesh.h"

#include "shaders/Shader.h"
#include <iostream>

// BufferObject structure to hold the VBO, EBO, and map VAOs to mesh groups
struct BufferObject {
    GLuint VBO;  // Shared VBO for all vertex data
    GLuint EBO;  // Shared EBO for all index data
    
    std::unordered_map < GLuint, std::vector<Mesh>> VAOMeshMap;

    // Constructor: Initialize the VBO and EBO
    BufferObject(std::vector<Mesh>& meshes) {
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

    void bufferData(std::vector<Mesh>& meshes) {

        std::vector<float> vertexData;
        std::vector<GLuint> indexData;

        GLuint indexOffset = 0;

        for (Mesh& mesh : meshes) {

            auto vertices = mesh.getVertices();
            auto indices = mesh.getIndices();

            if (!mesh.getVertices().empty()) {
                vertexData.insert(vertexData.end(), vertices.begin(), vertices.end());
            }
        
            if (!mesh.getIndices().empty()) {
                indexData.insert(indexData.end(), indices.begin(), indices.end());

                mesh.setStartingIndex(indexOffset);

                std::cout << indexOffset << std::endl;

                indexOffset += indices.size();
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);
    }

    // Destructor: Clean up the buffers
    ~BufferObject() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void draw() {
        for (const auto& pair : VAOMeshMap) {
            GLuint vao = pair.first;
            const std::vector<Mesh>& meshes = pair.second;

            glBindVertexArray(vao);

            for (const Mesh& mesh : meshes) {

                std::cout << mesh.getStartingIndex() << std::endl;
;
                glDrawElements(
                    GL_TRIANGLES, 
                    mesh.getIndices().size(), 
                    GL_UNSIGNED_INT, 
                    (void*)(mesh.getStartingIndex() * sizeof(GLuint))
                );

                debug();
            }
        }
        glBindVertexArray(0);
    }

    void debug(){
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }
    }

};


