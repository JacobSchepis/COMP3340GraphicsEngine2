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

	void generateStaticShadowMap();
	void generateDynamicShadowMap();

	std::vector<Light*> getLights();

private:
	std::vector<Light*> staticLightsVector;
	std::vector<Light*> dynamicLightsVector;

	Shader* shadowMapShader = new Shader("shaders/ShadowMap_vert.glsl", "shaders/ShadowMap_frag.glsl");

	int staticLightIndex = 0;
};