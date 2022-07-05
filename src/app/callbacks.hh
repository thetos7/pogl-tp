#pragma once
// glew first
#include <GL/glew.h>
// then others
#include <GLFW/glfw3.h>

namespace pogl
{
    void on_framebuffer_resize(GLFWwindow *window, int width, int height);
    void on_error(int error, const char *description);
    void on_key_update(GLFWwindow *window, int key, int scancode, int action,
                      int mods);
    void on_focus(GLFWwindow *window, int focused);
    void on_mouse_button(GLFWwindow *window, int button, int action, int mods);
} // namespace pogl
