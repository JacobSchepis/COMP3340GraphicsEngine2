#include "graphics/MeshRenderer.h"

#include <iostream>

MeshRenderer::MeshRenderer() {}

MeshRenderer::MeshRenderer(Mesh& mesh, std::vector<Texture>& newTextures)
    : mesh(mesh), textures(newTextures)
{}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::setup() {
    transform = Transform();
}

void MeshRenderer::render(Shader* shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
    
        shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    shader->setMat4("model", transform.getModel());

    // draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}