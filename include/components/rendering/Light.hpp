#pragma once

#include "components/abstract/IComponent.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <GL/glew.h>
#include "shaders/Shader.h"

#include <iostream>

// Enumeration of light source types
enum LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

class Light :public IComponent {
public:
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

    //shadow stuff

    GLuint shadowMapFBO;
    GLuint shadowMapTexture;
    GLuint shadowWidth;
    GLuint shadowHeight;


	// constructor
    Light(LightType type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float intensity)
        : type(type), ambient(ambient), diffuse(diffuse), specular(specular), intensity(intensity), shadowWidth(4096), shadowHeight(4096)
    {
		// initialise the default of light source
        if (type == LightType::DIRECTIONAL) {
             direction = glm::vec3(0.3f, -0.99f, 0.3f);
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

        position = glm::vec3(0, 20, 0);

        initializeShadowMap(shadowWidth, shadowHeight);
    }

	// Setting lighting attributes in the shader
    void applyLightToShader(class Shader& shader, const std::string& uniformName) {

        // Setting the basic attributes of light
        shader.setVec3(uniformName + ".color", ambient);
        //shader.setVec3(uniformName + ".diffuse", diffuse);
        //shader.setVec3(uniformName + ".specular", specular);
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

    void initializeShadowMap(int shadowWidth, int shadowHeight) {
        // Create FBO
        glGenFramebuffers(1, &shadowMapFBO);

        // Create shadow map texture
        glGenTextures(1, &shadowMapTexture);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // Attach depth texture as the FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
        glDrawBuffer(GL_NONE);  // We don't need color output
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glm::mat4 getLightSpaceMatrix() const {
        // Set up orthographic projection for directional light
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

        // Set up view matrix from light's perspective (position and direction)
        glm::mat4 lightView = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

        // Combine light projection and view matrices
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        return lightSpaceMatrix; 
    }


    // destructor
    virtual ~Light() {};
};