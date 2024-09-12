#include "graphics/Renderer.h"
#include <GL/glew.h>

#include <glm/glm.hpp>                  // Core GLM functionalities
#include <glm/gtc/matrix_transform.hpp>  // For glm::perspective, glm::rotate, glm::translate, etc.
#include <glm/gtc/type_ptr.hpp>          // For glm::value_ptr (to pass matrices to shaders)


Renderer::Renderer() : activeCamera(nullptr)
{}

Renderer::~Renderer() 
{
    activeCamera = nullptr;
}


void Renderer::render(Shader* shader) {
    // Clear the screen with a color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4& view = activeCamera->getViewMatrix();

    const glm::mat4& projection = activeCamera->getProjectionMatrix();

    shader->Use();

    GLuint viewLoc = glGetUniformLocation(shader->Program, "view");
    GLuint projLoc = glGetUniformLocation(shader->Program, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    for (BufferObject* bufferObject : m_bufferObjects)
        bufferObject->draw(shader);
}

void Renderer::queueMeshIntoBufferObject(Mesh* mesh) {
    m_meshToBufferObjectQueue.push_back(mesh);
}

void Renderer::pushMeshesToBuffer() {
    BufferObject* newBufferObject = new BufferObject(m_meshToBufferObjectQueue);
    m_bufferObjects.push_back(newBufferObject);
    m_meshToBufferObjectQueue.clear();
}

void Renderer::setActiveCamera(Camera* camera) {
    activeCamera = camera;
}