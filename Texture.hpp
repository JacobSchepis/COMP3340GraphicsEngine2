#pragma once

#include <GL/glew.h>
#include <string>

struct Texture {
	GLuint id;
	std::string filePath;

	Texture(GLuint textureId, const std::string& path)
		: id(textureId), filePath(path)
	{}
};