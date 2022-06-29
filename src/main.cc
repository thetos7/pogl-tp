// include glew first
#include <GL/glew.h>
// then others
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

#include "matrix4/matrix4.hh"
#include "shader_program/shader_program.hh"
#include "utils/gl_check.hh"
#include "utils/utils.hh"

// enables animation
#define ANIMATED 0
// enables color
#define COLOR 0

using namespace pogl;

std::unique_ptr<ShaderProgram> shader;
GLuint main_vao_id;

#if COLOR
const std::vector<GLfloat> vertex_color_data{
    // face Up
    // tri1
    1, 0, 0, //
    1, 0, 0, //
    1, 0, 0, //
    // tri2
    0.5, 0, 0, //
    0.5, 0, 0, //
    0.5, 0, 0, //

    // face Down
    // tri1
    0, 0, 1, //
    0, 0, 1, //
    0, 0, 1, //
    // tri2
    0, 0, 0.5, //
    0, 0, 0.5, //
    0, 0, 0.5, //

    // face West
    // tri1
    0, 1, 0, //
    0, 1, 0, //
    0, 1, 0, //
    // tri2
    0, 0.5, 0, //
    0, 0.5, 0, //
    0, 0.5, 0, //

    // face East
    // tri1
    1, 1, 0, //
    1, 1, 0, //
    1, 1, 0, //
    // tri2
    0.5, 0.5, 0, //
    0.5, 0.5, 0, //
    0.5, 0.5, 0, //

    // face North
    // tri1
    1, 0, 1, //
    1, 0, 1, //
    1, 0, 1, //
    // tri2
    0.5, 0, 0.5, //
    0.5, 0, 0.5, //
    0.5, 0, 0.5, //

    // face South
    // tri1
    0, 1, 1, //
    0, 1, 1, //
    0, 1, 1, //
    // tri2
    0, 0.5, 0.5, //
    0, 0.5, 0.5, //
    0, 0.5, 0.5, //

    // North plane
    // tri1
    0.7, 0.7, 0.7, //
    0.7, 0.7, 0.7, //
    0.7, 0.7, 0.7, //
    // tri2
    0.3, 0.3, 0.3, //
    0.3, 0.3, 0.3, //
    0.3, 0.3, 0.3, //
};
#endif // COLOR
const std::vector<GLfloat> vertex_position_data{
    // face Up
    // tri1
    1.0, 1.0, 1.0, // v8
    -1.0, 1.0, 1.0, // v7
    1.0, -1.0, 1.0, // v6
    // tri2
    1.0, -1.0, 1.0, // v6
    -1.0, 1.0, 1.0, // v7
    -1.0, -1.0, 1.0, // v5

    // face Down
    // tri1
    1.0, -1.0, -1.0, // v2
    -1.0, 1.0, -1.0, // v3
    1.0, 1.0, -1.0, // v4
    // tri2
    -1.0, 1.0, -1.0, // v3
    1.0, -1.0, -1.0, // v2
    -1.0, -1.0, -1.0, // v1

    // face West
    // tri1
    -1.0, 1.0, 1.0, // v7
    1.0, 1.0, 1.0, // v8
    1.0, 1.0, -1.0, // v4
    // tri2
    -1.0, 1.0, 1.0, // v7
    1.0, 1.0, -1.0, // v4
    -1.0, 1.0, -1.0, // v3

    // face East
    // tri1
    1.0, -1.0, -1.0, // v2
    1.0, -1.0, 1.0, // v6
    -1.0, -1.0, 1.0, // v5
    // tri2
    -1.0, -1.0, -1.0, // v1
    1.0, -1.0, -1.0, // v2
    -1.0, -1.0, 1.0, // v5

    // face North
    // tri1
    1.0, -1.0, -1.0, // v1
    1.0, 1.0, 1.0, // v7
    1.0, -1.0, 1.0, // v6
    // tri2
    1.0, -1.0, -1.0, // v1
    1.0, 1.0, -1.0, // v3
    1.0, 1.0, 1.0, // v7

    // face South
    // tri1
    -1.0, -1.0, 1.0, // v5
    -1.0, 1.0, 1.0, // v7
    -1.0, -1.0, -1.0, // v1
    // tri2
    -1.0, 1.0, -1.0, // v3
    -1.0, -1.0, -1.0, // v1
    -1.0, 1.0, 1.0, // v7

    //------------------------
    // North Plane
    // tri1
    0.0, -1.0, 2.0, // v1
    0.0, 1.0, 4.0, // v2
    0.0, -1.0, 4.0, // v3
    // tri2
    0.0, -1.0, 2.0, // v1
    0.0, 1.0, 2.0, // v4
    0.0, 1.0, 4.0, // v2
};

/* to make a plane:
 *  (0, 1) A---------B (1, 1)
 *         |       _/|
 *         |    __/  |
 *         |  _/     |
 *         |_/       |
 *  (0, 0) C---------D (1, 0)
 *
 *  trangle vertices should be sent in counter-clockwise order,
 * buffer should look like:
 * C
 * B
 * A
 *
 * C
 * D
 * B
 */

void display();
void window_resize(int width, int height);
void keyboard(unsigned char key, int x, int y);

#if ANIMATED
void timer(int value);
#endif // ANIMATED

bool init_glut(int &argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Test OpenGL - POGL");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
    glutKeyboardFunc(keyboard);
#if ANIMATED
    glutTimerFunc(20, timer, 0);
#endif // ANIMATED
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
#if COLOR
    shader = ShaderProgram::make_program(
        "../resources/shaders/vert_color/vertex.glsl",
        "../resources/shaders/vert_color/fragment.glsl");
#else
    shader = ShaderProgram::make_program(
        "../resources/shaders/uniform/vertex.glsl",
        "../resources/shaders/uniform/fragment.glsl");
#endif // COLOR
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

    const auto position_location = glGetAttribLocation(prog, "vPosition");
#if COLOR
    auto color_location = glGetAttribLocation(prog, "vColor");
#endif

    GLuint vertex_buffers[1 + COLOR];
    glGenBuffers(1 + COLOR, vertex_buffers);
    CHECK_GL_ERROR();

    // position
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[0]);
    CHECK_GL_ERROR();
    glBufferData(GL_ARRAY_BUFFER, vertex_position_data.size() * sizeof(GLfloat),
                 vertex_position_data.data(), GL_STATIC_DRAW);
    CHECK_GL_ERROR();
    glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(position_location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const auto model_transform_location =
        glGetUniformLocation(prog, "model_transform_matrix");

    const auto transform = Matrix4::identity();

    glUniformMatrix4fv(model_transform_location, 1, GL_TRUE, transform.data());

#if COLOR
    // color
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, vertex_color_data.size() * sizeof(GLfloat),
                 vertex_color_data.data(), GL_STATIC_DRAW);
    CHECK_GL_ERROR();
    glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(color_location);
    CHECK_GL_ERROR();

    glBindVertexArray(0);
    CHECK_GL_ERROR();
#endif // COLOR

    return true;
}

bool init_POV()
{
    const auto model_view_matrix = Matrix4::look_at( //
        1.5, 1.5, 1.5, // eye
        0., 0., 0., // center
        0., 0., 1. // up
    );
    const auto projection_matrix =
        Matrix4::frustum(-0.5, 0.5, -0.5, 0.5, 0.1, 100);

    const auto prog = shader->get_program();
    const auto view_matrix_loc =
        glGetUniformLocation(prog, "view_transform_matrix");
    const auto projection_matrix_loc =
        glGetUniformLocation(prog, "projection_matrix");

    glUniformMatrix4fv(view_matrix_loc, 1, GL_TRUE, model_view_matrix.data());
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_TRUE,
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
    glDrawArrays(GL_TRIANGLES, 0, vertex_position_data.size());
    CHECK_GL_ERROR();
    glBindVertexArray(0);
    CHECK_GL_ERROR();

    glutSwapBuffers();
}

#if ANIMATED
void timer(int value)
{
    float time = value * 0.001;

    GLfloat x_pos = 2.0 * std::cos(M_PI * time);
    GLfloat y_pos = 2.0 * std::sin(M_PI * time);
    GLfloat z_pos = 1.5 * std::cos(time);

    const auto model_view_matrix = Matrix4::look_at( //
        x_pos, y_pos, z_pos, // eye
        0, 0, 0, // center
        0, 0, 1 // up
    );

    const auto prog = shader->get_program();

    const auto view_matrix_loc =
        glGetUniformLocation(prog, "view_transform_matrix");

    glUniformMatrix4fv(view_matrix_loc, 1, GL_TRUE, model_view_matrix.data());

    glutPostRedisplay();
    glutTimerFunc(20, timer, value + 20);
}
#endif // ANIMATED

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
