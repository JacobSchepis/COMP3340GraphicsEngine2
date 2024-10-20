#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    // Offset the position slightly along the normal direction for the outline
    vec3 offsetPosition = aPos + aNormal * 0.2;  // Push vertex along normal by 2%

    gl_Position = projection * view * model * vec4(offsetPosition, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}
