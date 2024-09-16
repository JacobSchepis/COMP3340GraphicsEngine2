#pragma once

#include "components/abstract/IComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public IComponent {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 model;

    Transform();

    void updateModelMatrix();

    glm::mat4 getModel();


};