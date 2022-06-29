#pragma once

#include <GLFW/glfw3.h>

#include "inputstate/inputstate.hh"

namespace pogl
{
    void update_cursor_capture(GLFWwindow *window, const InputState &input);
} // namespace pogl
