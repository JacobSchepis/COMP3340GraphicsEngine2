#version 330 core
out vec4 FragColor;

#define NUM_LIGHTS 2 // Define the number of light sources (adjust this to match your number of lights)

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
    vec3 norm = normalize(Normal);  // Normalize the interpolated normal
    vec3 viewDir = normalize(viewPos - FragPos);  // Direction from fragment to the camera

    vec3 result = vec3(0.0);  // Initialize the result color as black

    // Loop through all the lights
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        // --- Ambient Lighting ---
        vec3 ambient = 
            light[i].ambient * 
            (hasDiffuseTexture ? texture(material.diffuseTexture, TexCoords).rgb : material.diffuseColor) * 
            light[i].intensity;

        // --- Diffuse Lighting ---
        vec3 lightDir = normalize(-light[i].direction);  // Direction from fragment to light
        float diff = max(dot(norm, lightDir), 0.0);  // Compute the diffuse factor
        vec3 diffuse = 
            light[i].diffuse * 
            diff * 
            (hasDiffuseTexture ? texture(material.diffuseTexture, TexCoords).rgb : material.diffuseColor) * 
            light[i].intensity;

        // --- Specular Lighting ---
        vec3 reflectDir = reflect(-lightDir, norm);   // Reflection vector
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);  // Compute the specular factor
        vec3 specular = 
            light[i].specular * 
            spec * (hasSpecularTexture ? texture(material.specularTexture, TexCoords).rgb : 
            material.specularColor) * 
            light[i].intensity;

        // Accumulate the lighting contribution from this light
        result += ambient + diffuse + specular;
    }

    // Output the final accumulated color
    FragColor = vec4(result, 1.0);  // Final color with full opacity
}
