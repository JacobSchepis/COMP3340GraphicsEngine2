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
    Model(char* modelPath)
    {
        loadModel(modelPath);
    }
    void Draw(Shader* shader);
private:
    // model data
    std::vector<MeshRenderer> meshRenderersVector;
    std::string directory;

    void loadModel(std::string modelPath);
    void processNode(aiNode* node, const aiScene* scene);
    std::vector<MeshRenderer> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

