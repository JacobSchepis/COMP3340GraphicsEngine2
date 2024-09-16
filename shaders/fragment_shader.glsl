#version 330 core
out vec4 FragColor;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    sampler2D diffuseTexture;
};

uniform Material material;

in vec2 TexCoords;

void main() {
    // Sample the texture
    vec4 diffuseMap = texture(material.diffuseTexture, TexCoords);

    // Combine the diffuse color and texture
    vec3 diffuse = material.diffuseColor * diffuseMap.rgb;

    // Specular color (for simplicity, no lighting here)
    vec3 specular = material.specularColor;

    FragColor = vec4(diffuse + specular, 1.0);
}
