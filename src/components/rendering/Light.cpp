#include "components/rendering/light.hpp"
#include "shaders/Shader.h" 

void Light::applyLightToShader(Shader& shader, const std::string& uniformName) {
    // Pass the light type to the shader
    shader.setInt(uniformName + ".type", type);
    
    // Setting the basic attributes of light
    shader.setVec3(uniformName + ".ambient", ambient);
    shader.setVec3(uniformName + ".diffuse", diffuse);
    shader.setVec3(uniformName + ".specular", specular);

    // Setting the specific arribute based on type of light source
    if (type == 0) {
        shader.setVec3(uniformName + ".direction", direction);
    } else if (type == 1) {
        shader.setVec3(uniformName + ".position", position);
        shader.setFloat(uniformName + ".constant", constant);
        shader.setFloat(uniformName + ".linear", linear);
        shader.setFloat(uniformName + ".quadratic", quadratic);
    } else if (type == 2) {
        shader.setVec3(uniformName + ".position", position);
        shader.setVec3(uniformName + ".spotDirection", spotDirection);
        shader.setFloat(uniformName + ".cutOff", cutOff);
        shader.setFloat(uniformName + ".outerCutOff", outerCutOff);
    }
}
