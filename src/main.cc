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
const std::vector<GLfloat> vertex_data{
    0.0,  1.0,  1.0, // v1
    1.0,  -1.0, 1.0, // v2
    -1.0, -1.0, 1.0 // v3
};

void display();
void window_resize(int width, int height);
void keyboard(unsigned char key, int x, int y);

bool init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Test OpenGL - POGL");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
    glutKeyboardFunc(keyboard);
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
    const auto prog = shader->get_program();

    glGenVertexArrays(1, &main_vao_id);
    CHECK_GL_ERROR();

    glBindVertexArray(main_vao_id);
    CHECK_GL_ERROR();

    auto position_location = glGetAttribLocation(prog, "position");
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    CHECK_GL_ERROR();
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    CHECK_GL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(GLfloat),
                 vertex_data.data(), GL_STATIC_DRAW);
    CHECK_GL_ERROR();
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(position_location);
    CHECK_GL_ERROR();

    glBindVertexArray(0);
    CHECK_GL_ERROR();

    return true;
}

bool init_POV()
{
    const auto model_view_matrix =
        Matrix4::look_at(0, 0, 0, 0.0, 0.0, 1, 0, 1, 0);
    const auto projection_matrix = Matrix4::frustum(-1, 1, -1, 1, 0.1, 20);

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

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        glutLeaveMainLoop();
        break;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR();

    glBindVertexArray(main_vao_id);
    CHECK_GL_ERROR();
    glDrawArrays(GL_TRIANGLES, 0, vertex_data.size());
    CHECK_GL_ERROR();
    glBindVertexArray(0);
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
