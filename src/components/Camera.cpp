#include "components/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include "components/Transform.h"

Camera::Camera(Entity* parent, float fov, float aspectRatio, float nearClip, float farClip)
    : IComponent(parent), fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip) {
}

glm::mat4 Camera::getViewMatrix() {
    Transform* transform = parent->getComponent<Transform>();

    // Initialize the view matrix to an identity matrix
    glm::mat4 view = glm::mat4(1.0f);

    // Apply rotation (rotation must be applied before translation)
    view = glm::rotate(view, glm::radians(transform->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch (X-axis)
    view = glm::rotate(view, glm::radians(transform->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw (Y-axis)
    view = glm::rotate(view, glm::radians(transform->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Roll (Z-axis)

    // Apply translation (inverse of position because it's a view matrix)
    view = glm::translate(view, -transform->position);

    return view;
}


glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}
