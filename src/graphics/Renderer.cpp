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

    shader->setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    shader->setVec3("viewPos", activeCamera->parent->getComponent<Transform>()->position);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for (Model* model: m_Models)
        model->Draw(shader);
}

void Renderer::setActiveCamera(Camera* camera) {
    activeCamera = camera;
}

void Renderer::addModel(Model* model) {
    m_Models.push_back(model);
}