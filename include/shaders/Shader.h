#pragma once

#include <string>
#include <GL/glew.h>

class Shader
{
public:
    GLuint Program;
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void Use();
private:
    std::string LoadShaderSource(const std::string& filePath);
    void CheckCompileErrors(GLuint shader, std::string type);
};