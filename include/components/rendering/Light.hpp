#pragma once

#include "components/abstract/IComponent.hpp"
#include <glm/glm.hpp>
#include <string>


// Enumeration of light source types
enum LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

class Light :public IComponent {

	// type of light source
	LightType type;

	// attribute
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float intensity;

	// Attributes for directional light
    glm::vec3 direction;

	// attribute for point light
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

	// Attributes for spot light
    glm::vec3 spotDirection;
    float cutOff;
    float outerCutOff;

public:
	// constructor
    Light(LightType type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity)
        : type(type), ambient(ambient), diffuse(diffuse), specular(specular), intensity(intensity) 
    {
		// initialise the default of light source
        if (type == LightType::DIRECTIONAL) {
             direction = glm::vec3(1.0f, -1.0f, 0.0f);
        } 
        
        else if (type == LightType::POINT) {
            position = glm::vec3(0.0f, 1.0f, 0.0f);
            constant = 1.0f;
            linear = 0.09f;
            quadratic = 0.032f;
        } 
        
        else if (type == LightType::SPOTLIGHT) {
            spotDirection = glm::vec3(0.0f, -1.0f, 0.0f);
            cutOff = glm::cos(glm::radians(12.5f));
            outerCutOff = glm::cos(glm::radians(15.0f));
        }
    }

	// Setting lighting attributes in the shader
    void applyLightToShader(class Shader& shader, const std::string& uniformName);

    // destructor
    virtual ~Light() {};
};