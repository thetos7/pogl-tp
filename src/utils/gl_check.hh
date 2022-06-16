#pragma once
#include <GL/glew.h>
#include <iostream>

inline bool checkGLError(const char *const file, int line)
{
    GLenum error_code = glGetError();
    if (error_code != GL_NO_ERROR)
    {
        std::cerr << "\e[31m[OpenGL Error]\e[39m " << file << " at line "
                  << line << ": got code `0x" << std::hex << ((int)error_code)
                  << std::dec << "` (" << ((int)error_code) << ")" << std::endl;
        return true;
    }
    return false;
}

/**
 * @brief Checks if the last opengl call raised an error, and prints an error
 * message containing the position in the file and error code. Expanded function
 * call returns a bool which is true when an error occured.
 *
 */
#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)