#version 330 core
out vec4 FragColor;

#define NUM_LIGHTS 1 // Define number of light sources

struct Material {
    vec3 diffuseColor;            // Base color if no texture is used
    vec3 specularColor;           // Specular color
    float shininess;              // Shininess for the specular calculation
    sampler2D diffuseTexture;     // Diffuse texture (optional)
    sampler2D specularTexture;    // Specular texture (optional)
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform Light light[NUM_LIGHTS];   // Multiple light sources
uniform vec3 viewPos;

uniform bool hasDiffuseTexture;       // Flag indicating whether a diffuse texture is used
uniform bool hasSpecularTexture;      // Flag indicating whether a specular texture is used

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    vec4 colour = vec4(material.diffuseColor, 1.0f);
    FragColor = colour;
}