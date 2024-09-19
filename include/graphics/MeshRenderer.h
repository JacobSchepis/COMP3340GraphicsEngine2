#pragma once

#include "components/Transform.h"

#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Material.hpp"

#include "shaders/Shader.h"

#include <vector>

class MeshRenderer {
public:
	MeshRenderer();
	MeshRenderer(Mesh& mesh, std::vector<Texture>& newTextures);
	~MeshRenderer();

	void setup();

	void render(Shader* shader);

	Transform transform;
	Mesh mesh;
	std::vector<Texture> textures;
	std::vector<Material> material;
};