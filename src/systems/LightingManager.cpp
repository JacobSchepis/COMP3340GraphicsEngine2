#include "systems/LightingManager.h"

#include <iostream>

LightingManager::~LightingManager() {
    for (auto light : staticLightsVector)
        light = nullptr;

    for (auto light : dynamicLightsVector)
        light = nullptr;
}

void LightingManager::addLight(Light* light, bool isStatic) {

    if (isStatic)
        staticLightsVector.push_back(light);
    else
        dynamicLightsVector.push_back(light);
}


void LightingManager::setStaticLights(Shader* shader) {
    shader->Use();
    int staticLightCount = staticLightsVector.size();
    for (int i = 0; i < staticLightCount; ++i) {
        std::string uniformName = "light[" + std::to_string(i) + "]";

        // Setting the basic attributes of light
        shader->setVec3(uniformName + ".color", staticLightsVector[i]->ambient);
        shader->setFloat(uniformName + ".intensity", staticLightsVector[i]->intensity);
        shader->setVec3(uniformName + ".direction", staticLightsVector[i]->direction);
    }
    staticLightIndex = staticLightCount;
}

void LightingManager::updateDynamicLights(Shader* shader) {
    
    for (int i = 0; i < dynamicLightsVector.size(); ++i) {
        // Apply the light properties to the shader

        std::string uniformName = "light[" + std::to_string(staticLightIndex + i) + "]";

        // Setting the basic attributes of light
        shader->setVec3(uniformName + ".color", dynamicLightsVector[i]->ambient);
        shader->setFloat(uniformName + ".intensity", dynamicLightsVector[i]->intensity);
        shader->setVec3(uniformName + ".direction", dynamicLightsVector[i]->direction);


        // Bind the shadow map for the current light to a unique texture unit starting from GL_TEXTURE7
        int textureUnit = 7 + i + staticLightIndex;  // Start from GL_TEXTURE7, and increment for each light

        //std::cout << textureUnit << std::endl;

        glActiveTexture(GL_TEXTURE0 + textureUnit);  // Activate the texture unit (e.g., GL_TEXTURE7, GL_TEXTURE8, etc.)
        glBindTexture(GL_TEXTURE_2D, dynamicLightsVector[i]->shadowMapTexture);  // Bind the shadow map texture

        // Set the corresponding sampler uniform in the shader to use this texture unit
        shader->setInt("shadowMap[" + std::to_string(i + staticLightIndex) + "]", textureUnit);  // Map shadowMap[i] to texture unit (7 + i)
        glm::mat4 lightSpaceMatrix = dynamicLightsVector[i]->getLightSpaceMatrix();
        shader->setMat4("lightSpaceMatrix[" + std::to_string(i + staticLightIndex) + "]", lightSpaceMatrix);
    }
}

void LightingManager::generateStaticShadowMap() {

}

void LightingManager::generateDynamicShadowMap() {

}

std::vector<Light*> LightingManager::getLights() {
    // Create a result vector to hold both static and dynamic lights
    std::vector<Light*> allLights;

    // Reserve space to avoid multiple reallocations (optional optimization)
    allLights.reserve(staticLightsVector.size() + dynamicLightsVector.size());

    // Insert static lights into the result vector
    allLights.insert(allLights.end(), staticLightsVector.begin(), staticLightsVector.end());

    // Insert dynamic lights into the result vector
    allLights.insert(allLights.end(), dynamicLightsVector.begin(), dynamicLightsVector.end());

    // Return the concatenated result
    return allLights;
}
