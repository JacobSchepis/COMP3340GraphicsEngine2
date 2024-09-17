#pragma once

#include "components/abstract/IComponent.hpp"

#include "graphics/Material.hpp"
#include "graphics/Mesh.hpp"
#include "graphics/Texture.hpp"

#include "components/Transform.h"
#include "shaders/Shader.h"

struct RenderObject {
	Transform transform;
	Mesh mesh;
	Material material;
	Texture texture;
};

class Model : public IComponent {
public:
	Model();

	~Model();

	void loadModel(std::string dir);
	void draw(Shader* shader);

private:
	std::vector<RenderObject> meshes;
	std::string directory;


};