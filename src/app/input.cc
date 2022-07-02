#include "input.hh"

#include "engine/engine.hh"
#include "inputstate/inputstate.hh"

namespace pogl
{
    void update_cursor_capture(GLFWwindow *window, const InputState &input)
    {
        glfwSetInputMode(window, GLFW_CURSOR,
                         input.capture_cursor ? GLFW_CURSOR_DISABLED
                                              : GLFW_CURSOR_NORMAL);
    }

    void mouse_update()
    {
        double xpos;
        double ypos;
        glfwGetCursorPos(Engine::instance().window, &xpos, &ypos);

        static double last_x = 0;
        static double last_y = 0;
        auto &input_state = get_input_state();

        const double diff_x = xpos - last_x;
        const double diff_y = ypos - last_y;
        last_x = xpos;
        last_y = ypos;

        input_state.mouse_x_axis = diff_x / 10.;
        input_state.mouse_y_axis = diff_y / 10.;
    }
} // namespace pogl
