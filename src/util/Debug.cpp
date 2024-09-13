#include "util/Debug.h"

#include <GL/glew.h>
#include <iostream>

void Debug::checkOpenGLError(const std::string& context) {
    GLenum errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:      error = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:     error = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:    error = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:   error = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:     error = "GL_OUT_OF_MEMORY"; break;
        default:                   error = "UNKNOWN ERROR"; break;
        }
        std::cerr << "OpenGL Error (" << context << "): " << error << std::endl;
    }
}
