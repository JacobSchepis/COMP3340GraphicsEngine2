#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 direction;
    glm::vec3 right;

    // Projection Parameters
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    // Constructor
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up,
        float fov, float aspectRatio, float nearClip, float farClip);

    // Get the View matrix (camera transformation)
    glm::mat4 getViewMatrix();

    // Get the Projection matrix (how the camera projects the world)
    glm::mat4 getProjectionMatrix();

    // Function to update camera direction based on position and target
    void updateCameraVectors();
};
