#version 330 core

layout (location = 0) in vec3 aPos;     // Vertex position (attribute location 0)
layout (location = 1) in vec3 aNormal;  // Vertex normal (attribute location 1)
layout (location = 2) in vec2 aTexCoords;  // Texture coordinates (attribute location 2)

out vec2 TexCoords;  // Pass texture coordinates to the fragment shader

uniform mat4 model;        // Model matrix (object space to world space)
uniform mat4 view;         // View matrix (world space to camera space)
uniform mat4 projection;   // Projection matrix (camera space to clip space)

void main() {
    // Calculate the final vertex position in clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Pass the texture coordinates to the fragment shader
    TexCoords = aTexCoords;
}
