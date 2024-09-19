#pragma once

#include "components/abstract/IComponent.hpp"

#include "components/Transform.h"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Material.hpp"

#include "shaders/Shader.h"

class MeshRenderer : public IComponent {
public:
	MeshRenderer();

	~MeshRenderer();

	void setup();

	void render(Shader* shader);
private:
	Transform* transform;
	Mesh* mesh;
	Texture* texture;
	Material* material;
};