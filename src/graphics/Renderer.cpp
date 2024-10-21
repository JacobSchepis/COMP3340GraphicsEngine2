#include "graphics/Renderer.h"
#include <GL/glew.h>

#include <glm/glm.hpp>                  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>          

#include "components/Transform.h"
#include <iostream>

Renderer::Renderer() : activeCamera(nullptr)
{}

Renderer::~Renderer() 
{
    activeCamera = nullptr;
}


void Renderer::render() {

    for (const auto& light : lightingManager.getLights()) {
        Shader* shadowShader = flagsToShader[ShadowMap];
        shadowShader->Use();

        // Set the light's view-projection matrix (lightSpaceMatrix)
        glm::mat4 lightSpaceMatrix = light->getLightSpaceMatrix();
        shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Render the scene to generate the shadow map
        for (const auto& modelPair : objectShaderFlags) {
            renderShadowMap(modelPair.first);
        }
    }

    // Clear the screen with a color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& shaderPair : flagsToShader) {
        auto shaderType = shaderPair.first;
        Shader* shader = shaderPair.second;

        shader->Use();
        setCamera(shader);
        lightingManager.updateDynamicLights(shader);

        for (const auto& modelPair : objectShaderFlags) {
            if (modelPair.second & shaderType)
                switch (shaderType) {
                case (PBR):
                    renderPBR(modelPair.first);
                    break;

                case (Outline):
                    renderOutline(modelPair.first);
                    break;

                case (ShadowMap):
                    break;
                }
            //other shader programs here
        }
    }
}

void Renderer::setCamera(Shader* shader) {
    shader->setVec3("viewPos", activeCamera->parent->getComponent<Transform>()->position);
    shader->setMat4("view", activeCamera->getViewMatrix());
    shader->setMat4("projection", activeCamera->getProjectionMatrix());
}

void Renderer::renderPBR(Model* model) {
    auto* shader = flagsToShader[PBR];

    for (auto& meshRenderer : model->meshRenderersVector) {
        // Activate texture unit 0 and bind the albedo map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.albedoTexture->id);  // Bind the texture ID for albedo
        glUniform1i(glGetUniformLocation(shader->Program, "material.albedoTexture"), 0);  // Pass the texture unit to the shader

        // Activate texture unit 1 and bind the normal map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.normalMap->id);  // Bind the texture ID for normal
        glUniform1i(glGetUniformLocation(shader->Program, "material.normalMap"), 1);  // Pass the texture unit to the shader

        // Activate texture unit 2 and bind the metallic map
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.metallicTexture->id);  // Bind the texture ID for metallic
        glUniform1i(glGetUniformLocation(shader->Program, "material.metallicTexture"), 2);  // Pass the texture unit to the shader

        // Activate texture unit 3 and bind the roughness map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.albedoTexture->id);  // Bind the texture ID for roughness
        glUniform1i(glGetUniformLocation(shader->Program, "material.roughnessTexture"), 3);  // Pass the texture unit to the shader

        // Activate texture unit 4 and bind the AO map
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.ambientTexture->id);  // Bind the texture ID for AO
        glUniform1i(glGetUniformLocation(shader->Program, "material.aoTexture"), 4);  // Pass the texture unit to the shader

        // Optionally, bind the emissive map to texture unit 5
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, meshRenderer.material.emissiveTexture->id);  // Bind the texture ID for emissive
        glUniform1i(glGetUniformLocation(shader->Program, "material.emissiveTexture"), 5);  // Pass the texture unit to the shader

        // If you have an environment map (cube map)
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, meshRenderer.material.environmentMap->id);  // Bind the environment cube map
        glUniform1i(glGetUniformLocation(shader->Program, "material.environmentMap"), 6);  // Pass the cube map texture unit to the shader


        shader->setMat4("model", meshRenderer.transform.getModel() * model->parent->getComponent<Transform>()->getModel());

        meshRenderer.render();
    }
}

void Renderer::renderOutline(Model* model) {

    Shader* shader = flagsToShader[Outline];

    // Enable depth testing but disable depth writing for the outline
    glEnable(GL_DEPTH_TEST); 
    //glDepthMask(GL_FALSE);  // Disable depth writing 

    for (auto& meshRenderer : model->meshRenderersVector) {
        glm::mat4 outlineModel = glm::scale(meshRenderer.transform.getModel() * model->parent->getComponent<Transform>()->getModel(), glm::vec3(1.05f));  // Scale by 5% larger
        
        shader->setMat4("model", outlineModel);

        meshRenderer.render();
    }

    glDepthMask(GL_TRUE);  // Re-enable depth writing

}

void Renderer::renderShadowMap(Model* model) {
    Shader* shadowShader = flagsToShader[ShadowMap];

    for (auto& meshRenderer : model->meshRenderersVector) {
        glm::mat4 modelMatrix = meshRenderer.transform.getModel();
        shadowShader->setMat4("model", modelMatrix);

        // Render the mesh with depth-only shader
        meshRenderer.render();
    }
}

void Renderer::setActiveCamera(Camera* camera) {
    activeCamera = camera;
}

void Renderer::addModel(Model* model, int shaderType) {
    objectShaderFlags[model] = shaderType;
}

void Renderer::addLight(Light* light, bool isStatic) {
    lightingManager.addLight(light, isStatic);
}

void Renderer::setStaticLights() {
    for (auto& pair : flagsToShader)
        lightingManager.setStaticLights(pair.second);
}