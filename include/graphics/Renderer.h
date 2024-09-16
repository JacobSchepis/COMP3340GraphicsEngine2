#pragma once

#include <vector>
#include "shaders/Shader.h"
#include "components/rendering/MeshRenderer.h"
#include "components/Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();

	void render(Shader* shader);

	void addMeshRenderer(MeshRenderer* meshRenderer);

	void setActiveCamera(Camera* camera);

private:
	std::vector<MeshRenderer*> m_MeshRenderers;

	Camera* activeCamera;
};