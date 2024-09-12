#include "graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
    float fov, float aspectRatio, float nearClip, float farClip)
    : position(position), target(target), up(up), fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip) {
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::updateCameraVectors() {
    // Recalculate the direction vector
    direction = glm::normalize(target - position);

    // Recalculate the right and up vector
    right = glm::normalize(glm::cross(direction, up));  // Right vector
    up = glm::normalize(glm::cross(right, direction));  // Recalculate the true up vector
}