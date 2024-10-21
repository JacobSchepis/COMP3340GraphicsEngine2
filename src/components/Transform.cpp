#include "components/Transform.h"

Transform::Transform() : position(0.0f), rotation(0.0f), scale(1.0f), model(glm::mat4(1.0f)) 
{
    updateModelMatrix();
}

void Transform::updateModelMatrix() {

    model = glm::mat4(1.0f);

    // Apply transformations in order: Scale -> Rotate -> Translate
    model = glm::translate(model, position);  // Translate to position
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotate around X-axis
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotate around Y-axis
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Rotate around Z-axis
    model = glm::scale(model, scale);  // Apply scaling
}

glm::mat4 Transform::getModel() {
    return model;
}