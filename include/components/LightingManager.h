#pragma once

#include <vector>
#include "components/rendering/Light.hpp"
#include "shaders/Shader.h"

class LightingManager {
private:
    std::vector<Light> staticLights;
    std::vector<Light> dynamicLights;

public:
    // 添加静态或动态光源
    void addLight(const Light& light);

    // initialise static light
    void initialiseStaticLighting(Shader* shader);

    // update dynamic light direction based on time
    void updateDynamicLighting(Shader* shader, float time);
};

