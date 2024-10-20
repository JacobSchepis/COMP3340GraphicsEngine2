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
        staticLightsVector[i]->applyLightToShader(*shader, "light[" + std::to_string(i) + "]");
    }
    staticLightIndex = staticLightCount;
}

void LightingManager::updateDynamicLights(Shader* shader) {
    for (int i = 0; i < dynamicLightsVector.size(); ++i) {
        dynamicLightsVector[i]->applyLightToShader(*shader, "light[" + std::to_string(staticLightIndex + i) + "]");
    }
}