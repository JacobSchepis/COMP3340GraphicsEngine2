#include "graphics/Renderer.h"
#include <GL/glew.h>
#include <SDL/SDL.h>

#include <glm/glm.hpp>                  
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>          

#include "components/Transform.h"
#include <iostream>

Renderer::Renderer() : activeCamera(nullptr)
{
    // initiate cloud shader
    cloudShader = new Shader("shaders/Cloud_vert.glsl", "shaders/Cloud_frag.glsl");
    if (!cloudShader->Program) {
        std::cout << "Failed to load cloud shader" << std::endl;
    }
    setupCloud();
}

Renderer::~Renderer() 
{
    activeCamera = nullptr;

    // clear cloud shader
    glDeleteVertexArrays(1, &cloudVAO);
    glDeleteBuffers(1, &cloudVBO);
    delete cloudShader;
}


void Renderer::render() {

    for (const auto& light : lightingManager.getLights()) {
        Shader* shadowShader = flagsToShader[ShadowMap];
        shadowShader->Use();

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, light->shadowWidth, light->shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, light->shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Set the light's view-projection matrix (lightSpaceMatrix)
        glm::mat4 lightSpaceMatrix = light->getLightSpaceMatrix();
        shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // Render the scene to generate the shadow map
        for (const auto& modelPair : objectShaderFlags) {
            renderShadowMap(modelPair.first);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Clear the screen with a color
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearColor(0.2f, 0.6f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glViewport(0, 0, 800, 600);
    glViewport(0, 0, 1024, 900);


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

    // 启用混合并渲染云层
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);  // 可能需要禁用深度测试

    cloudShader->Use();
    float time = SDL_GetTicks() * 0.001f;  // 转换毫秒到秒
    // 设置云层shader的uniform变量
    cloudShader->setMat4("view", activeCamera->getViewMatrix());
    cloudShader->setMat4("projection", activeCamera->getProjectionMatrix());
    cloudShader->setFloat("time", time);

    cloudShader->setFloat("cloudHeight", 60.0f);  // 设置云层高度
    
    renderCloud();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
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

void Renderer::setupCloud() {
    // 创建一个简单的四边形用于渲染云层
    float quadVertices[] = {
        // 位置            // 纹理坐标
        -500.0f,  500.0f,  0.0f,  1.0f,  // 顶点1，左上角
        -500.0f, -500.0f,  0.0f,  0.0f,  // 顶点2，左下角
         500.0f, -500.0f,  1.0f,  0.0f,  // 顶点3，右下角

        -500.0f,  500.0f,  0.0f,  1.0f,  // 顶点4，左上角
         500.0f, -500.0f,  1.0f,  0.0f,  // 顶点5，右下角
         500.0f,  500.0f,  1.0f,  1.0f   // 顶点6，右上角
    };

    glGenVertexArrays(1, &cloudVAO);
    glGenBuffers(1, &cloudVBO);

    glBindVertexArray(cloudVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // 位置属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // 纹理坐标属性
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void Renderer::renderCloud() {
    glBindVertexArray(cloudVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}