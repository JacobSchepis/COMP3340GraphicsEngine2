#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap;  // The shadow map texture

void main() {
    // Get depth value from the shadow map
    float depthValue = texture(shadowMap, TexCoords).r;
    
    // Visualize depth (convert depth to grayscale)
    FragColor = vec4(vec3(depthValue), 1.0);  // Grayscale color
}
