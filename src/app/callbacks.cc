#include "callbacks.hh"

#include <iostream>

#include "app/input.hh"
#include "engine/engine.hh"
#include "inputstate/inputstate.hh"
#include "utils/gl_check.hh"

namespace pogl
{

    void on_error(int, const char *description)
    {
        std::cerr << LOG_ERROR << "GLFW ERROR: " << description << "\n";
    }

    void on_framebuffer_resize(GLFWwindow *, int width, int height)
    {
        glViewport(0, 0, width, height);
        CHECK_GL_ERROR();
        const auto aspect_ratio = width / (float)height;
        Engine::instance().update_perspective(aspect_ratio);
    }

    void on_key_update(GLFWwindow *window, int key, int, int action, int)
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

    void on_focus(GLFWwindow *, int focused)
    {
        get_input_state().focused = focused != 0;
    }
} // namespace pogl
