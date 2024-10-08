#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <assimp/scene.h>
#include <stb_image.h>


struct Texture {
	GLuint id;
	std::string type;
	aiString path;

    void loadTextureFromFile(const std::string& path) {

        std::string filePath = "../../../resources/models/backpack/" + path;

        GLuint textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
        std::cout << filePath << std::endl;
        if (data) {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        }
        else {
            std::string failureReason = stbi_failure_reason();
            std::cerr << "Failed to load texture: " << failureReason << std::endl;
        }

        stbi_image_free(data);
        id = textureId;
    }
};