#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseTexture;
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light[NUM_LIGHTS];        // for multiple light scources
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0); // initialise result

    for (int i = 0; i < NUM_LIGHTS; ++i) { // Iterate over all light sources
        // Ambient lighting
        vec3 ambient = light[i].ambient * texture(material.diffuseTexture, TexCoords).rgb;
        
        // Diffuse lighting
        vec3 lightDir = normalize(-light[i].direction); // Directional light assumes direction, not position
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light[i].diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
        
        // Specular lighting (Phong model)
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = light[i].specular * spec; // Assuming a white specular highlight

        // Sum the contribution of each light source
        result += ambient + diffuse + specular;
    }
    
    FragColor = vec4(result, 1.0);
}
