#pragma once

#include <GL/glew.h>
#include <iostream>

class Debug {
public:
    void checkOpenGLError(const std::string& context);
};