#version 330 core
out vec4 FragColor;

#define NUM_LIGHTS 1 // Define number of light sources

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

uniform Material material;
uniform Light light[NUM_LIGHTS];   // Multiple light sources
uniform vec3 viewPos;

uniform bool hasDiffuseTexture;
uniform bool hasSpecularTexture;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    // --- Ambient Lighting ---
    vec3 ambient = 
        light[0].ambient * 
        (hasDiffuseTexture ? texture(material.diffuseTexture, TexCoords).rgb : material.diffuseColor) * 
        light[0].intensity;

    // --- Diffuse Lighting ---
    vec3 norm = normalize(Normal);  // Normalize the interpolated normal
    vec3 lightDir = normalize(-light[0].direction);  // Direction from fragment to light
    float diff = max(dot(norm, lightDir), 0.0);  // Compute the diffuse factor
    vec3 diffuse = 
        light[0].diffuse * 
        diff * 
        (hasDiffuseTexture ? texture(material.diffuseTexture, TexCoords).rgb : material.diffuseColor) * 
        light[0].intensity;

    // --- Specular Lighting ---
    vec3 viewDir = normalize(viewPos - FragPos);  // Direction from fragment to the camera
    vec3 reflectDir = reflect(-lightDir, norm);   // Reflection vector
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);  // Compute the specular factor
    vec3 specular = 
        light[0].specular * 
        spec * (hasSpecularTexture ? texture(material.specularTexture, TexCoords).rgb : 
        material.specularColor) * 
        light[0].intensity;

    // --- Final Color Calculation ---
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);  // Output the final color with full opacity
}
