#pragma once

#include "components/Transform.h"

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Material.hpp"

#include "shaders/Shader.h"

#include <vector>

class MeshRenderer {
public:
	MeshRenderer(Mesh& mesh, Material& material);
	~MeshRenderer();

	void setup();

	void render(Shader* shader);

	Transform transform;
	Mesh mesh;
	Material material;
};