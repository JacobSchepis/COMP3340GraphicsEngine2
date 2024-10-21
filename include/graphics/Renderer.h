#pragma once

#include <vector>
#include <unordered_map>
#include "shaders/Shader.h"
#include "components/Camera.h"
#include "components/rendering/Model.h"
#include "systems/LightingManager.h"


class Renderer {
public:
	enum ShaderType {
		PBR = 1 << 0,        // 0001
		Outline = 1 << 1, // 0010
		ShadowMap = 1 << 2, // 0100
		// Add more shaders as needed
	};

	Renderer();
	~Renderer();

	void render();

	void addModel(Model* model, int shaderType);

	void addLight(Light* light, bool isStatic);
	void setStaticLights();

	void setActiveCamera(Camera* camera);

private:
	std::unordered_map<Model*, int> objectShaderFlags;

	std::unordered_map<ShaderType, Shader*> flagsToShader = {
		{ShadowMap, new Shader("shaders/ShadowMap_vert.glsl", "shaders/ShadowMap_frag.glsl")},
		{PBR, new Shader("shaders/PBR_vert.glsl", "shaders/test_frag.glsl")},
		{Outline, new Shader("shaders/Outline_vert.glsl", "shaders/Outline_frag.glsl")}
	};

	Camera* activeCamera;
	LightingManager lightingManager = LightingManager();

	void renderPBR(Model* model);
	void renderOutline(Model* model);
	void renderShadowMap(Model* model);

	void setCamera(Shader* shader);
};