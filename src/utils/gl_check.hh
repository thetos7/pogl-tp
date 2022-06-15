#pragma once
#include <GL/glew.h>
#include <iostream>

inline bool checkGLError(const char *const file, int line)
{
    GLenum error_code = glGetError();
    if (error_code != GL_NO_ERROR)
    {
        std::cerr << "\e[31m[OpenGL Error]\e[39m " << file << " at line "
                  << line << ": got code `0x" << std::hex << ((int)error_code) << "`"
                  << std::endl;
        return true;
    }
    return false;
}

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)