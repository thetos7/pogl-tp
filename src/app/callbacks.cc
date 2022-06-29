#include "callbacks.hh"

#include <iostream>

#include "app/input.hh"
#include "inputstate/inputstate.hh"
#include "utils/gl_check.hh"

namespace pogl
{

    void on_error(int error, const char *description)
    {
        std::cerr << "GLFW ERROR: " << description << "\n";
    }

    void on_window_resize(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
        CHECK_GL_ERROR();
    }

    void on_key_update(GLFWwindow *window, int key, int scancode, int action,
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

    void on_mouse_move(GLFWwindow *window, double xpos, double ypos)
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
} // namespace pogl
