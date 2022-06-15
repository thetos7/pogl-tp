// include glew first
#include <GL/glew.h>
// then others
#include <GL/freeglut.h>
#include <iostream>

#include "matrix4/matrix4.hh"
#include "shader_program/shader_program.hh"
#include "utils/gl_check.hh"
#include "utils/utils.hh"

using namespace pogl;

void display();
void window_resize(int width, int height);

bool init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Test OpenGL - POGL");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
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
    auto prog = ShaderProgram::make_program(
        "../resources/shaders/uniform/uniform.vert",
        "../resources/shaders/uniform/uniform.frag");
    prog.use();
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

void window_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    std::cout << "intialising GLUT...\n";
    init_glut(argc, argv);
    std::cout << "intialising GLEW...\n";
    init_glew();
    std::cout << "intialising GL...\n";
    init_GL();
    std::cout << "intialising shaders...\n";
    init_shaders();
    std::cout << "intialising objects...\n";
    init_object();
    std::cout << "intialising POV...\n";
    init_POV();
    std::cout << "launching\n";
    // glutMainLoop();
    std::cout << "exiting\n";

    return 0;
}
