#pragma once

#include <glm/glm.hpp>
#include "IComponent.h"

class IComponent;
class Entity;


class Camera : public IComponent{
public:
    float fov;
    float aspectRatio;
    float nearClip;
    float farClip;

    // Constructor
    Camera(Entity* parent, float fov, float aspectRatio, float nearClip, float farClip);

    // Get the View matrix (camera transformation)
    glm::mat4 getViewMatrix();

    // Get the Projection matrix (how the camera projects the world)
    glm::mat4 getProjectionMatrix();
};
