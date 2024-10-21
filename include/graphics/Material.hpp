#pragma once

#include "graphics/Texture.hpp"

#include <glm/glm.hpp>

struct Material {

    Texture* albedoTexture;
    Texture* roughnessTexture;
    Texture* metallicTexture;
    Texture* ambientTexture;
    Texture* emissiveTexture;
    Texture* heightMap;
    Texture* normalMap;
    Texture* opacityMap;
    Texture* environmentMap;

    float shininess;

    Material(std::string path) :
        albedoTexture(new Texture(path + "/albedo.png")),
        roughnessTexture(new Texture(path + "/roughness.png")),
        metallicTexture(new Texture(path + "/metallic.png")),
        ambientTexture(new Texture(path + "/ao.png")),
        emissiveTexture(new Texture(path + "/emissive.png")),
        heightMap(new Texture(path + "/hight.png")),
        normalMap(new Texture(path + "/normalMap.png")),
        opacityMap(new Texture(path + "/opacity.png")),
        environmentMap(new Texture(path + "/evironmentMap.png")),

        shininess(14.0f)
    {}
};