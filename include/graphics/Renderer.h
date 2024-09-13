#pragma once

#include <vector>
#include "shaders/Shader.h"
#include "graphics/BufferObject.h"
#include "components/Camera.h"


class Renderer {
public:
	Renderer();
	~Renderer();

	void render(Shader* shader);

	void queueMeshIntoBufferObject(Mesh* mesh);

	void pushMeshesToBuffer();

	void setActiveCamera(Camera* camera);

private:
	std::vector<BufferObject*> m_bufferObjects;

	std::vector<Mesh*> m_meshToBufferObjectQueue;

	Camera* activeCamera;
};