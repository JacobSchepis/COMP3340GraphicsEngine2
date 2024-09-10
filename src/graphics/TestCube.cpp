#include "graphics/TestCube.h"

TestCube::TestCube(std::vector<float>& vertices, std::vector<GLuint>& indices) : VAO(0), vertices(vertices), indices(indices)
{
    glGenBuffers(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

TestCube::~TestCube()
{
}

void TestCube::setupVAO(GLuint sharedVBO, GLuint sharedEBO) {
    // Generate and bind the VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Bind the shared VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, sharedVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sharedEBO);

    // Set vertex attribute pointers (assuming position data only)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO
    glBindVertexArray(0);
}