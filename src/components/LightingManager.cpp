#include "components/LightingManager.h"
//#include "Shader.h"  // 假设 Shader 类用于和着色器进行通信

void LightingManager::addLight(const Light& light) {
    if (light.isStatic()) {
        staticLights.push_back(light);
    } else {
        dynamicLights.push_back(light);
    }
}

// 移除静态或动态光源
//    void removeLight(const Light& light) {
//        // 从静态或动态光源中移除
//        if (light.isStatic()) {
//            staticLights.erase(std::remove(staticLights.begin(), staticLights.end(), light), staticLights.end());
//        } else {
//            dynamicLights.erase(std::remove(dynamicLights.begin(), dynamicLights.end(), light), dynamicLights.end());
//        }
//    }

// 初始化静态光源（只需要设置一次）
void LightingManager::initialiseStaticLighting(Shader* shader) {
    for (const auto& light : staticLights) {
        // 将每个光源应用到着色器
        // light.applyLightToShader(*shader, "staticLights");
        light.applyLightToShader(*shader, "light");
    }
}

// 更新动态光源（每帧更新一次）
void LightingManager::updateDynamicLighting(Shader* shader, float time) {
    for (auto& light : dynamicLights) {
        light.updateLightDirection(time);  // 根据时间动态更新光源方向
        light.applyLightToShader(*shader, "dynamicLights");
    }
}