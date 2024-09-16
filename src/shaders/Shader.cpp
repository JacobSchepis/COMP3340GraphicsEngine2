#include "shaders/Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from the filePath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        vertexCode = LoadShaderSource(vertexPath);
        fragmentCode = LoadShaderSource(fragmentPath);
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    GLuint vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    Program = glCreateProgram();
    glAttachShader(Program, vertex);
    glAttachShader(Program, fragment);
    glLinkProgram(Program);
    CheckCompileErrors(Program, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

std::string Shader::LoadShaderSource(const std::string& filePath)
{
    std::ifstream file;
    std::stringstream buffer;

    file.open(filePath);
    if (!file.is_open())
    {
        throw std::ifstream::failure("Failed to open shader file");
    }
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type == "VERTEX" || type == "FRAGMENT")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
    else if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}

void Shader::Use()
{
    glUseProgram(Program);
}

// Utility function to set a uniform int in the shader
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
}

// Utility function to set a uniform float in the shader
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
}

// Utility function to set a uniform vec3 in the shader
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(Program, name.c_str()), 1, glm::value_ptr(value));
}

// Utility function to set a uniform vec3 with individual components
void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(Program, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const{
    GLuint loc = glGetUniformLocation(Program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
