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
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0); // Initialize result

    // Choose the diffuse color based on whether a texture is available
    vec3 diffuseColor;
    if (material.hasDiffuseTexture) {
        diffuseColor = texture(material.diffuseTexture, TexCoords).rgb;
    } else {
        diffuseColor = material.diffuseColor;  // Use base color if no texture is provided
    }


    // Iterate over all light sources
    for (int i = 0; i < NUM_LIGHTS; ++i) {
        // Ambient lighting
        vec3 ambient = light[i].ambient * diffuseColor;

        // Diffuse lighting
        vec3 lightDir = normalize(-light[i].direction);  // Directional light assumes direction, not position
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light[i].diffuse * diff * diffuseColor;

        // Specular lighting (Phong model)
        vec3 specularColor;
        if (material.hasSpecularTexture) {
            specularColor = texture(material.specularTexture, TexCoords).rgb;
        } else {
            specularColor = material.specularColor;  // Use base specular color if no texture is provided
        }

        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);  // Use material's shininess
        vec3 specular = light[i].specular * spec * specularColor;

        // Sum the contribution of each light source
        result += ambient + diffuse + specular;
    }

    FragColor = vec4(result, 1.0);  // Final color with full opacity
}
