#pragma once

#include "components/abstract/IComponent.hpp"

#include "shaders/Shader.h"
#include "graphics/MeshRenderer.h"

#include <vector>
#include <string>

#include <assimp/scene.h>

class Model : public IComponent
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader* shader);
private:
    // model data
    std::vector<MeshRenderer> meshRenderersVector;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    MeshRenderer processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};