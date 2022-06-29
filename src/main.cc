// include glew first
#include <GL/glew.h>
// then others
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include "camera/camera.hh"
#include "inputstate/inputstate.hh"
#include "matrix4/matrix4.hh"
#include "shader_program/shader_program.hh"
#include "utils/gl_check.hh"
#include "utils/utils.hh"

// enables color
#define COLOR 0

using namespace pogl;

std::unique_ptr<ShaderProgram> shader;
std::shared_ptr<Camera> camera;
GLuint main_vao_id;
GLFWwindow *window;

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
void window_resize(GLFWwindow *window, int width, int height);

void error_callback(int error, const char *description);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
void cursor_callback(GLFWwindow *window, double xpos, double ypos);

void update_cursor_capture(GLFWwindow *window, const InputState &input);

bool init_glfw()
{
    if (!glfwInit())
    {
        std::cerr << "ERROR: GLFW initialisation failed.\n";
        return false;
    }
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    window = glfwCreateWindow(700, 700, "Test OpenGl - POGL (GLFW)", nullptr,
                              nullptr);
    if (!window)
    {
        std::cerr << "ERROR: GLFW Window creation failed.\n";
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_resize);

    update_cursor_capture(window, get_input_state());

    glfwSetCursorPosCallback(window, cursor_callback);

    return true;
}

void error_callback(int error, const char *description)
{
    std::cerr << "GLFW ERROR: " << description << "\n";
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
    camera = std::make_shared<Camera>(Vector3(2.5, 0, 0), 0.0, M_PI);
    const auto projection_matrix =
        Matrix4::frustum(-0.5, 0.5, -0.5, 0.5, 1, 100);

    const auto prog = shader->get_program();
    const auto view_matrix_loc =
        glGetUniformLocation(prog, "view_transform_matrix");
    const auto projection_matrix_loc =
        glGetUniformLocation(prog, "projection_matrix");

    glUniformMatrix4fv(view_matrix_loc, 1, GL_TRUE,
                       camera->get_transform().data());
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_TRUE,
                       projection_matrix.data());
    return true;
}

void window_resize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR();
}

void update_cursor_capture(GLFWwindow *window, const InputState &input)
{
    std::cerr << "Cursor capture active? " << input.capture_cursor << "\n";
    glfwSetInputMode(window, GLFW_CURSOR,
                     input.capture_cursor ? GLFW_CURSOR_DISABLED
                                          : GLFW_CURSOR_NORMAL);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods)
{
    auto &input_state = get_input_state();

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;
    case GLFW_KEY_C:
        if (action == GLFW_PRESS)
        {
            input_state.capture_cursor = !input_state.capture_cursor;
            update_cursor_capture(window, input_state);
        }
        break;
    case GLFW_KEY_W:
        input_state.forward = (action != GLFW_RELEASE);
        break;
    case GLFW_KEY_A:
        input_state.left = (action != GLFW_RELEASE);
        break;
    case GLFW_KEY_S:
        input_state.backward = (action != GLFW_RELEASE);
        break;
    case GLFW_KEY_D:
        input_state.right = (action != GLFW_RELEASE);
        break;
    case GLFW_KEY_SPACE:
        input_state.up = (action != GLFW_RELEASE);
        break;
    case GLFW_KEY_LEFT_SHIFT:
        input_state.down = (action != GLFW_RELEASE);
        break;
    }
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    static double last_x = 0;
    static double last_y = 0;
    auto &input_state = get_input_state();

    const double diff_x = xpos - last_x;
    const double diff_y = ypos - last_y;
    last_x = xpos;
    last_y = ypos;

    input_state.mouse_x_axis = diff_x / 5.;
    input_state.mouse_y_axis = diff_y / 5.;
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
}

void update()
{
    static double last_tick = 0;
    const double now = glfwGetTime();
    const double delta = now - last_tick;
    last_tick = now;

    // update objects
    camera->update(delta);

    auto &input_state = get_input_state();
    input_state.mouse_x_axis = 0;
    input_state.mouse_y_axis = 0;
    // update rendering parameters
    const auto prog = shader->get_program();

    const auto view_transform_loc =
        glGetUniformLocation(prog, "view_transform_matrix");

    glUniformMatrix4fv(view_transform_loc, 1, GL_TRUE,
                       camera->get_transform().data());
}

int main(int argc, char *argv[])
{
    std::cout << "initialising GLFW...\n";
    init_glfw();
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
    std::cout << "Camera controls:\n";
    std::cout << "Z/S: Forward/Backwards\n";
    std::cout << "Q/D: Left/Right\n";
    std::cout << "Space/Left Shift: Up/Down\n";
    std::cout << "C: Toggle cursor capture\n";

    while (!glfwWindowShouldClose(window))
    {
        update();
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "exiting\n";

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
