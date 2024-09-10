#include "graphics/Renderer.h"
#include <GL/glew.h>

Renderer::Renderer() : m_VBO(0), m_EBO(0)
{

}

Renderer::~Renderer()
{

}

void Renderer::sendObjectsToBuffer(TestCube& testCube) {
    std::vector<TestCube> testCubes;
    testCubes.push_back(testCube);
    sendObjectsToBuffer(testCubes);
}

void Renderer::sendObjectsToBuffer(std::vector<TestCube>& testCubes) {
    std::vector<float> vertices;
    std::vector<GLuint> indices;

    // Combine vertices and adjust indices for each cube
    int offset = 0;
    for (auto& testCube : testCubes) {
        // Add the vertices of the current TestCube to the combined list
        vertices.insert(vertices.end(), testCube.vertices.begin(), testCube.vertices.end());

        // Add the indices, adjusting the offset for each object
        for (auto index : testCube.indices) {
            indices.push_back(index + offset);  // Offset the index based on previously added vertices
        }

        offset += testCube.vertices.size() / 3;  // Increase offset by the number of vertices
    }

    // Generate the VBO and EBO for the combined vertex and index data
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Now each TestCube will configure its own VAO
    for (auto& testCube : testCubes) {
        testCube.setupVAO(m_VBO, m_EBO);
    }
}

void Renderer::render(Shader* shader, SDL_Window* window) {
    // Clear the screen with a color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Use();
    for (auto& testCube : testCubes) {
        glBindVertexArray(testCube.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    } 


    // Swap buffers
    SDL_GL_SwapWindow(window);
}