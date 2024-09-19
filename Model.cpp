#include "components/rendering/Model.h"

Model::Model(char path) {

}

Model::~Model() {

}

void Model::draw(Shader* shader) {
	for (auto meshRenderer : meshRenderers) {
		meshRenderer.render(shader);
	}
}

void Model::loadModel(char path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshRenderers.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

MeshRenderer Model::processMesh(aiMesh* mesh, aiScene* scene) {
    Mesh mesh = Mesh(std::vector<Vertex>(), std::vector<GLuint>());
    Texture texture = Texture(0, "");
    Material material = Material(glm::vec3(0), glm::vec3(0), 0.0f);

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        glm::vec3 normals;
        normals.x = mesh->mNormals[i].x;
        normals.y = mesh->mNormals[i].y;
        normals.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = texCoord;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

    }


}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {

}