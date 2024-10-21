#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define NUM_LIGHTS 1  // Adjust this based on the number of lights

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace[NUM_LIGHTS];  // Output position in light space for shadow mapping

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix[NUM_LIGHTS];  // Light space matrix for shadow mapping

void main()
{
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calculate normal in world space
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    // Pass texture coordinates to the fragment shader
    TexCoords = aTexCoords;
    
    // Calculate the final position of the vertex (world to clip space)
    gl_Position = projection * view * vec4(FragPos, 1.0);

    // Calculate the fragment position in light space for each light (used for shadow mapping)
    for (int i = 0; i < NUM_LIGHTS; i++) {
        FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);  // Transform to light space
    }
}
