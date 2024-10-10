#include "graphics/MeshRenderer.h"

#include <iostream>

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material)
    : mesh(mesh), material(material)
{}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::setup() {
    transform = Transform();
}

void MeshRenderer::render(Shader* shader)
{
    // Bind material properties
    shader->setVec3("material.diffuseColor", material.diffuseColor);
    shader->setVec3("material.specularColor", material.specularColor);
    shader->setFloat("material.shininess", material.shininess);

    // Set the flags for textures
    shader->setBool("hasDiffuseTexture", material.hasDiffuseTexture);
    shader->setBool("hasSpecularTexture", material.hasSpecularTexture);

    // Bind diffuse texture if available
    if (material.hasDiffuseTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.diffuseTexture->id);
        shader->setInt("material.diffuseTexture", 0);  // Texture unit 0 for diffuse
    }

    // Bind specular texture if available
    if (material.hasSpecularTexture) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material.specularTexture->id);
        shader->setInt("material.specularTexture", 1);  // Texture unit 1 for specular
    }

    // Set the model matrix
    shader->setMat4("model", transform.getModel());

    // Draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

