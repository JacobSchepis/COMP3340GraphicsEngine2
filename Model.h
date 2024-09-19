#pragma once

#include "components/abstract/IComponent.hpp"

#include "graphics/MeshRenderer.h"
#include "shaders/Shader.h"

class Model : public IComponent {
public:
	Model(char path);
	void draw(Shader* shader);


private:

	std::vector<MeshRenderer> meshRenderers;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	void loadModel(char path);

	void processNode(aiNode* node, const aiScene* scene);
	MeshRenderer processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


};