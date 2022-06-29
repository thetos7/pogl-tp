#pragma once
// glew first
#include <GL/glew.h>
// then others
#include <GLFW/glfw3.h>

namespace pogl
{
    void window_resize(GLFWwindow *window, int width, int height);
    void error_callback(int error, const char *description);
    void key_callback(GLFWwindow *window, int key, int scancode, int action,
                      int mods);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);
} // namespace pogl
