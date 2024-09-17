#pragma once

#include "components/abstract/IComponent.hpp"

#include "components/rendering/Texture.hpp"

#include <glm/glm.hpp>

struct Material {
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;

    Texture* diffuseTexture;
    Texture* specularTexture;

    Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : diffuseColor(diffuse), specularColor(specular), shininess(shininess),
        diffuseTexture(nullptr), specularTexture(nullptr) {}
};