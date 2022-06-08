#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glew.h>
#include <iostream>

#include "matrix4/matrix4.hh"
#include "utils/utils.hh"

inline bool checkGLError(const char *const file, int line)
{
    GLenum error_code = glGetError();
    if (error_code != GL_NO_ERROR)
    {
        std::cerr << "\e[31m[OpenGL Error]\e[39m " << file << " at line "
                  << line << ": got code `" << ((int)error_code) << "`"
                  << std::endl;
    }
}

#define CHECK_GL_ERROR() checkGLError(__FILE__, __LINE__)

bool init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Test OpenGL - POGL");
    return true;
}

bool init_glew()
{
    return (glewInit() == GLEW_OK);
}

bool init_GL()
{
    constexpr int PIXEL_BYTE_ALIGNEMENT_LEN = 1; // 1 byte for 8-bit color
    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    CHECK_GL_ERROR();
    glEnable(GL_CULL_FACE);
    CHECK_GL_ERROR();
    glClearColor(0.5, 0.5, 0.5, 1.0);
    CHECK_GL_ERROR();
    glPixelStorei(GL_PACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
    CHECK_GL_ERROR();
    glPixelStorei(GL_UNPACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
    CHECK_GL_ERROR();

    return true;
}

bool init_shaders()
{
    return true;
}

bool init_object()
{
    return true;
}

bool init_POV()
{
    return true;
}

int main(int argc, char *argv[])
{
    using namespace pogl;

    init_glut(argc, argv);
    init_glew();
    init_GL();
    init_shaders();
    init_object();
    init_POV();
    glutMainLoop();

    return 0;
}
