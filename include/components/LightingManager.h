#pragma once

#include <vector>
#include "components/rendering/Light.hpp"
#include "shaders/Shader.h"

class LightingManager {
private:
    std::vector<Light> staticLights;  // 静态光源列表
    std::vector<Light> dynamicLights; // 动态光源列表

public:
    // 添加静态或动态光源
    void addLight(const Light& light);

    // 初始化静态光源（只需设置一次）
    void initialiseStaticLighting(Shader* shader);

    // 更新动态光源（每帧更新一次）
    void updateDynamicLighting(Shader* shader, float time);
};

