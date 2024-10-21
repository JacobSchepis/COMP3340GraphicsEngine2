#include "components/LightingManager.h"
//#include "Shader.h"  // 假设 Shader 类用于和着色器进行通信

void LightingManager::addLight(const Light& light) {
    if (light.isStatic()) {
        staticLights.push_back(light);
    } else {
        dynamicLights.push_back(light);
    }
}

// remove light source
//    void removeLight(const Light& light) {
//        // remove
//        if (light.isStatic()) {
//            staticLights.erase(std::remove(staticLights.begin(), staticLights.end(), light), staticLights.end());
//        } else {
//            dynamicLights.erase(std::remove(dynamicLights.begin(), dynamicLights.end(), light), dynamicLights.end());
//        }
//    }

// initialise static light
void LightingManager::initialiseStaticLighting(Shader* shader) {
    int index = 0;  // Index used to traverse the static light array
    for (const auto& light : staticLights) {
        // Generate unique array index names for each light, such as "staticLights[0]", "staticLights[1]", etc.
        std::string lightName = "staticLights[" + std::to_string(index) + "]";
        light.applyLightToShader(*shader, lightName);
        ++index;  // Incrementing index
    }
}

// update dynamic light direction based on time
void LightingManager::updateDynamicLighting(Shader* shader, float time) {
    int index = 0;  // Index used to traverse the dynamic light array
    for (auto& light : dynamicLights) {
        light.updateLightDirection(time);  // Dynamically update the light source direction based on time

        // Generate unique array index names for each light, such as "dynamicLights[0]", "dynamicLights[1]", etc.
        std::string lightName = "dynamicLights[" + std::to_string(index) + "]";
        light.applyLightToShader(*shader, lightName);

        ++index;  // Incrementing index
    }
}