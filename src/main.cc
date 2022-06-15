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

std::unique_ptr<ShaderProgram> shader;
GLuint main_vao_id;

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
    shader = ShaderProgram::make_program(
        "../resources/shaders/uniform/vertex.glsl",
        "../resources/shaders/uniform/fragment.glsl");
    shader->use();

    const auto prog = shader->get_program();
    const auto obj_color_loc = glGetUniformLocation(prog, "obj_color");
    glUniform4f(obj_color_loc, 1., 0., 0., 1.);
    return true;
}

bool init_object()
{
    std::vector<float> vertex_data{
        0.5,  0.75, 0.5, // v1
        0.75, 0.25, 0.5, // v2
        0.25, 0.25, 0.5 // v3
    };
    glGenVertexArrays(1, &main_vao_id);
    CHECK_GL_ERROR();

    glBindVertexArray(main_vao_id);
    CHECK_GL_ERROR();
    return true;
}

bool init_POV()
{
    const auto model_view_matrix = Matrix4::identity();
    const auto projection_matrix = Matrix4::identity();

    const auto prog = shader->get_program();
    const auto model_view_matrix_loc =
        glGetUniformLocation(prog, "model_view_matrix");
    const auto projection_matrix_loc =
        glGetUniformLocation(prog, "projection_matrix");

    glUniformMatrix4fv(model_view_matrix_loc, 1, false,
                       model_view_matrix.data());
    glUniformMatrix4fv(projection_matrix_loc, 1, false,
                       projection_matrix.data());
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

    glBindVertexArray(main_vao_id);
    CHECK_GL_ERROR();

    glutSwapBuffers();
}

int main(int argc, char *argv[])
{
    std::cout << "initialising GLUT...\n";
    init_glut(argc, argv);
    std::cout << "initialising GLEW...\n";
    init_glew();
    std::cout << "initialising GL...\n";
    init_GL();
    std::cout << "initialising shaders...\n";
    init_shaders();
    std::cout << "initialising objects...\n";
    init_object();
    std::cout << "initialising POV...\n";
    init_POV();
    std::cout << "launching\n";
    glutMainLoop();
    std::cout << "exiting\n";

    return 0;
}
