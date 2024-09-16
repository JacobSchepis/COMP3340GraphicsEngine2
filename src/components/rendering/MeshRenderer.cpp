#include "components/rendering/MeshRenderer.h"

MeshRenderer::MeshRenderer() 
    : transform(nullptr), mesh(nullptr), texture(nullptr), material(nullptr)
{}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::setup() {
    transform = parent->getComponent<Transform>();
    mesh = parent->getComponent<Mesh>();
    texture = parent->getComponent<Texture>();
    material = parent->getComponent<Material>();
}

void MeshRenderer::render(Shader* shader) {
    // Bind the VAO stored in the Mesh
    glBindVertexArray(mesh->VAO);

    // Bind the texture (if available)
    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->id);
        shader->setInt("material.diffuseTexture", 0);
    }

    // Set material properties in the shader
    shader->setVec3("material.diffuseColor", material->diffuseColor);
    shader->setVec3("material.specularColor", material->specularColor);
    shader->setFloat("material.shininess", material->shininess);

    shader->setMat4("model", transform->getModel());

    // Draw the mesh using the indices from the EBO
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    // Unbind the VAO
    glBindVertexArray(0);
}