#include "scripts/CameraController.h"

#include <glm/glm.hpp>
#include "input/InputManager.h"

#include <iostream>

CameraController::CameraController() : transform(nullptr) {
}

void CameraController::start() {
    transform = parent->getComponent<Transform>();
}

void CameraController::update(){
    applyMovement();
    applyRotation();
    transform->updateModelMatrix();
}

void CameraController::applyMovement()
{
    // Get the input vector for WASD movement
    auto rawInputVector = InputManager::Instance().getWasdInputVector();  // Assumes this returns a 2D vector with x: left/right, y: forward/backward
    glm::vec2 inputVector = glm::vec2(rawInputVector.y, rawInputVector.x);

    // Convert the yaw (rotation.y) into a forward direction (on the XZ plane)
    float lookDirectionEuler = glm::radians(transform->rotation.y);  // Assume rotation.y is the yaw in radians
    glm::vec2 lookDirectionCartesian = glm::vec2(glm::cos(lookDirectionEuler), glm::sin(lookDirectionEuler));

    // Calculate the right direction by rotating the forward direction by 90 degrees
    glm::vec2 rightDirectionCartesian = glm::vec2(-lookDirectionCartesian.y, lookDirectionCartesian.x);  // Perpendicular to the forward vector

    // Compute the final movement direction in the XZ plane
    // inputVector.y controls forward/backward movement (W/S keys)
    // inputVector.x controls left/right movement (A/D keys)
    glm::vec2 moveDirection = lookDirectionCartesian * inputVector.y + rightDirectionCartesian * -inputVector.x;

    int verticalMovement = InputManager::Instance().getVerticalInput();

    // Apply the movement (assuming deltaTime is available for smooth movement)
    // float deltaTime = Time::GetDeltaTime();  // Assuming a delta time function exists
    transform->position += glm::vec3(moveDirection.x, verticalMovement, moveDirection.y);
}

void CameraController::applyRotation() {
    // Get the mouse delta from the InputManager
    glm::vec2& mouseDelta = InputManager::Instance().getMouseDelta();

    // Sensitivity for controlling how fast the camera rotates
    float sensitivity = 0.1f;

    // Adjust yaw (y-axis rotation) based on horizontal mouse movement
    float yaw = transform->rotation.y;
    yaw += mouseDelta.x * sensitivity;

    // Adjust pitch (x-axis rotation) based on vertical mouse movement
    float pitch = transform->rotation.x;
    pitch += mouseDelta.y * sensitivity;

    // Clamp the pitch to prevent the camera from flipping upside down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new forward vector based on yaw and pitch
    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    // Apply the updated yaw and pitch back to the transform's rotation
    transform->rotation = glm::vec3(pitch, yaw, 0.0f);  // Assuming rotation.x is pitch and rotation.y is yaw
}

