#pragma once

#include <vector>
#include "components/rendering/Light.hpp"
#include "shaders/Shader.h"

class LightingManager {
public:
	~LightingManager();

	void addLight(Light* light, bool isStatic);
	void setStaticLights(Shader* shader);
	void updateDynamicLights(Shader* shader);

private:
	std::vector<Light*> staticLightsVector;
	std::vector<Light*> dynamicLightsVector;

	int staticLightIndex = 0;
};