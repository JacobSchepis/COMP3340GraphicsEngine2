#pragma once

#include <GL/glew.h>
#include <iostream>

static class Debug {
public:
    static void checkOpenGLError(const std::string& context);
};