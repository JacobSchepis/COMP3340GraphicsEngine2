#include "components/rendering/light.hpp"
#include "shaders/Shader.h" 

void Light::applyLightToShader(Shader& shader, const std::string& uniformName) {
    // Setting the basic attributes of light
    shader.setVec3(uniformName + ".ambient", ambient);
    shader.setVec3(uniformName + ".diffuse", diffuse);
    shader.setVec3(uniformName + ".specular", specular);
    shader.setFloat(uniformName + ".intensity", intensity);

    // Setting the specific arribute based on type of light source
    if (type == DIRECTIONAL) {
        shader.setVec3(uniformName + ".direction", direction);
    }
    
    else if (type == POINT) {
        shader.setVec3(uniformName + ".position", position);
        shader.setFloat(uniformName + ".constant", constant);
        shader.setFloat(uniformName + ".linear", linear);
        shader.setFloat(uniformName + ".quadratic", quadratic);
    } 
    
    else if (type == SPOTLIGHT) {
        shader.setVec3(uniformName + ".position", position);
        shader.setVec3(uniformName + ".spotDirection", spotDirection);
        shader.setFloat(uniformName + ".cutOff", cutOff);
        shader.setFloat(uniformName + ".outerCutOff", outerCutOff);
    }
}
