#include "components/rendering/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

void Model::Draw(Shader* shader) {
    for (auto& meshRenderer : meshRenderersVector)
        meshRenderer.render(shader);
}

void Model::loadModel(std::string modelPath)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(modelPath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = modelPath.substr(0, modelPath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        std::vector<MeshRenderer> subMeshRenderers = processMesh(mesh, scene);

        // Append the subMeshRenderers into meshRenderersVector
        meshRenderersVector.insert(meshRenderersVector.end(), subMeshRenderers.begin(), subMeshRenderers.end());
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<MeshRenderer> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::cout << "processing new mesh" << std::endl;


    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshRenderer> meshRenderers;

    // Process vertices as usual
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {

        Vertex vertex;
        glm::vec3 vector;

        // Process position, normals, and texture coordinates
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::cout << "Number of faces: " << mesh->mNumFaces * 3 << std::endl; 
    std::cout << "Number of indices: " << indices.size() << std::endl;

    // Process materials
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex];
        Material material;

        // Load diffuse color or texture
        aiColor3D color(0.f, 0.f, 0.f);
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        {
            material.diffuseColor = glm::vec3(color.r, color.g, color.b);  // Set base color
            //std::cout << "Diffuse Color: " << color.r << " " << color.g << " " << color.b << std::endl;
        }

        // Load diffuse texture
        std::vector<Texture> diffuseMaps = loadMaterialTextures(aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
        if (!diffuseMaps.empty()) {
            material.diffuseTexture = &diffuseMaps[0];  // Set the first diffuse texture
            material.hasDiffuseTexture = true;
        }
        else {
            material.hasDiffuseTexture = false;
        }

        //std::cout << "Diffuse Texture = " << material.hasDiffuseTexture << std::endl;

        // Load specular color or texture
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color))
        {
            material.specularColor = glm::vec3(color.r, color.g, color.b);  // Set specular color
        }

        // Load specular texture
        std::vector<Texture> specularMaps = loadMaterialTextures(aiMaterial, aiTextureType_SPECULAR, "texture_specular");
        if (!specularMaps.empty()) {
            material.specularTexture = &specularMaps[0];  // Set the first specular texture
            material.hasSpecularTexture = true;
        }
        else {
            material.hasSpecularTexture = false;
        }

        //std::cout << "Specular Texture = " << material.hasSpecularTexture << std::endl;

        // Load shininess
        float shininess;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_SHININESS, shininess))
        {
            material.shininess = shininess;
        }
        else
        {
            material.shininess = 32.0f;  // Default shininess value
        }

        // Create MeshRenderer with the current mesh and material
        MeshRenderer newMeshRenderer = MeshRenderer(Mesh(vertices, indices), material);
        meshRenderers.push_back(newMeshRenderer);
    }

    return meshRenderers;
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        texture.loadTextureFromFile(str.C_Str());
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);
    }
    return textures;
}