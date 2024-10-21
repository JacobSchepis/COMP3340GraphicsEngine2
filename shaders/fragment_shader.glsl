#version 330 core
out vec4 FragColor;

#define NUM_STATIC_LIGHTS 1 // define number of static light source
#define NUM_DYNAMIC_LIGHTS 1 // define number of dynamic light source

struct Material {
    sampler2D diffuseTexture;
};

struct Light {
    int type; // 0: Directional, 1: Point, 2: Spotlight

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Directional
    vec3 direction;

    // Point and Spotlight
    vec3 position;
    float constant;
    float linear;
    float quadratic;

    // Spotlight
    vec3 spotDirection;
    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Light staticLights[NUM_STATIC_LIGHTS];
uniform Light dynamicLights[NUM_DYNAMIC_LIGHTS];
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Extract lighting calculation functions to reduce duplicate code
vec3 calculateLighting(Light light, vec3 norm, vec3 viewDir) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir;
    float attenuation = 1.0;

    // Ambient lighting
    ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb;

    if (light.type == 0) { // Directional Light
        lightDir = normalize(-light.direction);
        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = light.specular * spec;       // Assuming a white specular highlight
    }
    else if (light.type == 1) { // Point Light
        lightDir = normalize(light.position - FragPos);
        // Calculate attenuation
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = light.specular * spec;       // Assuming a white specular highlight
        // Apply attenuation
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }
    else if (light.type == 2) { // Spotlight
        lightDir = normalize(light.position - FragPos);
        // Calculate attenuation
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        // Calculating light cone angle
        float theta = dot(lightDir, normalize(-light.spotDirection));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
        // Specular lighting
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specular = light.specular * spec;       // Assuming a white specular highlight
        // Apply attenuation and Light cone intensity
        attenuation *= intensity;
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    // return result
    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);  // initialise result

    // 处理静态光源
    for (int i = 0; i < NUM_STATIC_LIGHTS; ++i) {
        result += calculateLighting(staticLights[i], norm, viewDir);
    }

    // 处理动态光源
    for (int i = 0; i < NUM_DYNAMIC_LIGHTS; ++i) {
        result += calculateLighting(dynamicLights[i], norm, viewDir);
    }

    FragColor = vec4(result, 1.0);
}
