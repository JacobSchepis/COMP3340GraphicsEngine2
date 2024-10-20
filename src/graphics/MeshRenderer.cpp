#include "graphics/MeshRenderer.h"

#include <iostream>

MeshRenderer::MeshRenderer(Mesh& mesh, Material& material)
    : mesh(mesh), material(material)
{}

MeshRenderer::~MeshRenderer() {}

void MeshRenderer::setup() {
    transform = Transform();
}

void MeshRenderer::render()
{
    // Draw mesh
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

