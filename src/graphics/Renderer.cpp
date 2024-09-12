#include "graphics/Renderer.h"
#include <GL/glew.h>

Renderer::Renderer() {}

Renderer::~Renderer() {}


void Renderer::render(Shader* shader) {
    // Clear the screen with a color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //shader->Use();
    //for (auto& bufferObject : m_bufferObjects)
    //    bufferObject.draw();

}

void Renderer::queueMeshIntoBufferObject(Mesh& mesh) {
    m_meshToBufferObjectQueue.push_back(mesh);
}

void Renderer::pushMeshesToBuffer() {
    //BufferObject newBufferObject = BufferObject(m_meshToBufferObjectQueue);
    //m_bufferObjects.push_back(newBufferObject);
    //m_meshToBufferObjectQueue.clear();
}