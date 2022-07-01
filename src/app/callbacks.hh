#pragma once
// glew first
#include <GL/glew.h>
// then others
#include <GLFW/glfw3.h>

namespace pogl
{
    void on_window_resize(GLFWwindow *window, int width, int height);
    void on_error(int error, const char *description);
    void on_key_update(GLFWwindow *window, int key, int scancode, int action,
                      int mods);
    void on_mouse_move(GLFWwindow *window, double xpos, double ypos);
    void on_focus(GLFWwindow *window, int focused);
} // namespace pogl
