#pragma once

#include "components/abstract/IComponent.hpp"
#include <glm/glm.hpp>
#include <string>
#include <iostream>


// Enumeration of light source types
//enum LightType {
//    DIRECTIONAL,
//    POINT,
//    SPOTLIGHT
//};

class Light :public IComponent {
	// type of light source
	//LightType type;
    int type;  // (0: DIRECTIONAL, 1: POINT, 2: SPOTLIGHT)

	// attribute
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool isStaticLight;

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
    Light(const std::string& typeStr, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, bool isStatic)
        : ambient(ambient), diffuse(diffuse), specular(specular), isStaticLight(isStatic) {
        
		// initialise the default of light source
        if (typeStr == "DIRECTIONAL") {
            type = 0;
            //direction = glm::vec3(-0.2f, -1.0f, -0.3f);
            direction = glm::vec3(1.0f, 0.0f, 0.0f);
        } else if (typeStr == "POINT") {
            type = 1;
            position = glm::vec3(0.0f, 1.0f, 0.0f);
            constant = 1.0f;
            linear = 0.09f;
            quadratic = 0.032f;
        } else if (typeStr == "SPOTLIGHT") {
            type = 2;
            spotDirection = glm::vec3(0.0f, -1.0f, 0.0f);
            cutOff = glm::cos(glm::radians(12.5f));
            outerCutOff = glm::cos(glm::radians(15.0f));
        }
    }

    // if is a static light source
    bool isStatic() const {
        return isStaticLight;
    }

    // Add an update function to dynamically adjust the light direction
    // Only update for directional light
    void updateLightDirection(float time) {
        if (type == 0) {
            // Control the speed of the sun's movement, 0.5f is the speed ratio 
            // according to the angle of change over time, 0.1f is the proportional factor that controls the rotation speed
            float angle = time * 0.5f;
            //Rotate the light in the XZ plane
            direction = glm::vec3(cos(angle), sin(angle), 0.0f);
        }
    }

	// Setting lighting attributes in the shader
    void applyLightToShader(class Shader& shader, const std::string& uniformName) const;

    // destructor
    virtual ~Light() {};
};