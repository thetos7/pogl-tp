#include "lifecycle.hh"

#include <cmath>
#include <iostream>

#include "callbacks.hh"
#include "input.hh"
#include "inputstate/inputstate.hh"
#include "utils/gl_check.hh"

// enables color
#define COLOR 0
#include "vertex_data.hh"

namespace pogl
{
    std::unique_ptr<ShaderProgram> shader;
    std::shared_ptr<Camera> camera;
    GLuint main_vao_id;
    GLFWwindow *window;

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
        window = glfwCreateWindow(700, 700, "Test OpenGl - POGL (GLFW)",
                                  nullptr, nullptr);
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
        glBufferData(GL_ARRAY_BUFFER,
                     vertex_position_data.size() * sizeof(GLfloat),
                     vertex_position_data.data(), GL_STATIC_DRAW);
        CHECK_GL_ERROR();
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CHECK_GL_ERROR();
        glEnableVertexAttribArray(position_location);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        const auto model_transform_location =
            glGetUniformLocation(prog, "model_transform_matrix");

        const auto transform = Matrix4::identity();

        glUniformMatrix4fv(model_transform_location, 1, GL_TRUE,
                           transform.data());

#if COLOR
        // color
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[1]);
        glBufferData(GL_ARRAY_BUFFER,
                     vertex_color_data.size() * sizeof(GLfloat),
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
} // namespace pogl
