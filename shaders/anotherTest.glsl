#version 330 core
out vec4 FragColor;

#define NUM_LIGHTS 1  // Define the number of light sources

struct Material {
    sampler2D albedoTexture;      // Albedo (base color) texture
    sampler2D normalMap;          // Normal map texture
    sampler2D roughnessTexture;   // Roughness texture
    sampler2D metallicTexture;    // Metallic texture
    sampler2D aoTexture;          // Ambient occlusion texture
    sampler2D emissiveTexture;    // Emissive map texture (if needed)
    samplerCube environmentMap;   // Environment map (for reflections)
};

struct Light {
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform Material material;
uniform Light light[NUM_LIGHTS];  // Array of light sources
uniform vec3 viewPos;             // Camera position
uniform vec3 lightPos[NUM_LIGHTS];  // Light positions for point lights

uniform sampler2D shadowMap[NUM_LIGHTS];  // Shadow maps for each light
uniform mat4 lightSpaceMatrix[NUM_LIGHTS];  // Light space matrices for each light

in vec3 FragPos;   // Position of the fragment
in vec3 Normal;    // Interpolated normal
in vec2 TexCoords; // Texture coordinates
//in vec3 TangentViewPos; // View position in tangent space for normal mapping
in vec4 FragPosLightSpace[NUM_LIGHTS];  // Fragment position in light space

// Helper function for Fresnel calculation
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Helper function for geometry calculation
float geometrySchlickGGX(float NdotV, float roughness) {
    float k = (roughness * roughness) / 2.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Helper function for normal distribution function
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265 * denom * denom;  // Use PI for the normalization

    return num / denom;
}

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;  // Perspective divide
    projCoords = projCoords * 0.5 + 0.5;  // Transform to [0, 1] range
    
    // Get the closest depth value from the shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    
    // Get the current depth of the fragment from the light's perspective
    float currentDepth = projCoords.z;
    
    // Apply a small bias to avoid shadow acne
    float bias = 0.005;
    
    // Determine if the fragment is in shadow
    float shadow = currentDepth == closestDepth ? 1.0 : 0.0;
    
    shadow = closestDepth > 0 ? 1.0 : 0.0;
    
    return shadow;
}


void main()
{
    // Retrieve properties from texture maps (PBR inputs)
    vec3 albedo = texture(material.albedoTexture, TexCoords).rgb;
    float roughness = texture(material.roughnessTexture, TexCoords).r;
    float metallic = texture(material.metallicTexture, TexCoords).r;

    // Normal mapping (optional, depending on implementation)
    vec3 N = normalize(Normal);  // Adjust this to use normal maps if needed
    vec3 viewDir = normalize(viewPos - FragPos);

    // F0 is the base reflectivity for non-metallic surfaces
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);  // Blend between metallic and non-metallic F0

    vec3 result = vec3(0.0);  // Initialize result color

    // Loop over lights and accumulate lighting
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(-light[i].direction);  // Direction of the light source
        vec3 radiance = light[i].color * light[i].intensity;  // Light radiance

        // --- Cook-Torrance BRDF Calculations ---
        vec3 H = normalize(viewDir + lightDir);  // Halfway vector between light and view direction
        float NDF = distributionGGX(N, H, roughness);  // Normal Distribution Function (GGX)
        float G = geometrySchlickGGX(max(dot(N, viewDir), 0.0), roughness) * geometrySchlickGGX(max(dot(N, lightDir), 0.0), roughness);
        vec3 F = fresnelSchlick(max(dot(H, viewDir), 0.0), F0);  // Fresnel term

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, viewDir), 0.0) * max(dot(N, lightDir), 0.0) + 0.001;  // Prevent division by 0
        vec3 specular = numerator / denominator;

        // --- Diffuse Lighting ---
        vec3 kD = vec3(1.0) - F;  // Energy conservation: diffuse is reduced by the specular amount
        kD *= 1.0 - metallic;     // Metallic surfaces do not have diffuse reflection

        float NdotL = max(dot(N, lightDir), 0.0);  // Lambertian diffuse factor
        vec3 diffuse = kD * albedo / 3.14159265;  // Diffuse reflection

        // --- Shadow Calculation ---
        float shadow = ShadowCalculation(FragPosLightSpace[i], shadowMap[i]);  // Get shadow factor

        // Accumulate the light contribution (include shadow factor)
        result = vec3(shadow, 0.0, 1.0 - shadow);  // Apply shadow to light
    }

    // Optional: Ambient lighting using the AO map
    vec3 ambient = texture(material.aoTexture, TexCoords).rgb * albedo;

    // Combine final result with ambient and emissive lighting
    vec3 emissive = texture(material.emissiveTexture, TexCoords).rgb;  // Emissive lighting
    FragColor = vec4(result, 1.0);  // Output the final color with full opacity
}
