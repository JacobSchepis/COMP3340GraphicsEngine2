#pragma once

#include <vector>
#include "shaders/Shader.h"
#include "components/Camera.h"
#include "components/rendering/Model.h"


class Renderer {
public:
	Renderer();
	~Renderer();

	void render(Shader* shader);

	void addModel(Model* model);

	void setActiveCamera(Camera* camera);

private:
	std::vector<Model*> m_Models;

	Camera* activeCamera;
};