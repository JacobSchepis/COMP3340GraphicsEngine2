#include "components/rendering/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

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
    //std::cout << "processing new mesh" << std::endl;


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


    MeshRenderer newMeshRenderer = MeshRenderer(Mesh(vertices, indices), Material(directory)); 
    meshRenderers.push_back(newMeshRenderer); 

    return meshRenderers;
}