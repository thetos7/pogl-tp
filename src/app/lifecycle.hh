#pragma once

// glew first
#include <GL/glew.h>
// then others
#include <GLFW/glfw3.h>
#include <memory>

#include "camera/camera.hh"
#include "shader_program/shader_program.hh"

namespace pogl
{
    extern std::unique_ptr<ShaderProgram> shader;
    extern std::shared_ptr<Camera> camera;
    extern GLuint main_vao_id;
    extern GLFWwindow *window;

    void display();
    bool init_glfw();
    bool init_glew();
    bool init_GL();
    bool init_shaders();
    bool init_object();
    bool init_POV();
    void update();
} // namespace pogl
