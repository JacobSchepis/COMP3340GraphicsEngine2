#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include "graphics/Mesh.h"

#include "shaders/Shader.h"

// BufferObject structure to hold the VBO, EBO, and map VAOs to mesh groups
struct BufferObject {
    GLuint VBO;  // Shared VBO for all vertex data
    GLuint EBO;  // Shared EBO for all index data
    
    std::unordered_map < GLuint, std::vector<Mesh*>> VAOMeshMap;

    // Constructor: Initialize the VBO and EBO
    BufferObject(std::vector<Mesh*> meshes);
    

    ~BufferObject();

    void bufferData(std::vector<Mesh*>& meshes);

    void draw(Shader* shader) const;
};


