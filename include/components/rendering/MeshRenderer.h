#pragma once

#include "components/abstract/IComponent.hpp"

#include "components/Transform.h"
#include "components/rendering/Mesh.hpp"
#include "components/rendering/Texture.hpp"
#include "components/rendering/Material.hpp"

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