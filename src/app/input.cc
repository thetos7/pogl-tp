#include "input.hh"

#include "inputstate/inputstate.hh"

namespace pogl
{
    void update_cursor_capture(GLFWwindow *window, const InputState &input)
    {
        glfwSetInputMode(window, GLFW_CURSOR,
                         input.capture_cursor ? GLFW_CURSOR_DISABLED
                                              : GLFW_CURSOR_NORMAL);
    }
} // namespace pogl
