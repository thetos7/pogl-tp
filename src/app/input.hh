#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "inputstate/inputstate.hh"

namespace pogl
{
    void update_cursor_capture(GLFWwindow *window, const InputState &input);
    void mouse_update();
} // namespace pogl
