#include "graphics/Renderer.h"
#include <GL/glew.h>

#include <glm/glm.hpp>                  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>          

#include "components/Transform.h"
#include <iostream>

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

    for (Model* model: m_Models)
        model->Draw(shader);
}

void Renderer::setActiveCamera(Camera* camera) {
    activeCamera = camera;
}

void Renderer::addModel(Model* model) {
    m_Models.push_back(model);
}