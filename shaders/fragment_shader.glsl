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
uniform Light light;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    // Ambient lighting
    vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); // Directional light assumes direction, not position
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
    
    // Specular lighting (Phong model)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec; // Assuming a white specular highlight
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
