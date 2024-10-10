#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
    GLuint Program;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void Use();

    // Utility function to set a uniform int in the shader
    void setInt(const std::string& name, int value) const;

    // Utility function to set a uniform float in the shader
    void setFloat(const std::string& name, float value) const;

    // Utility function to set a uniform vec3 in the shader
    void setVec3(const std::string& name, const glm::vec3& value) const;

    // Utility function to set a uniform vec3 with individual components
    void setVec3(const std::string& name, float x, float y, float z) const;

    void setMat4(const std::string& name, const glm::mat4& value) const;

    void setBool(const std::string& name, const bool value) const;

private:
    std::string LoadShaderSource(const std::string& filePath);
    void CheckCompileErrors(GLuint shader, std::string type);
};