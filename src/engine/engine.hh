#pragma once
// glew first
#include <GL/glew.h>
// others
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include <vector>

#include "camera/camera.hh"
#include "properties/renderable.hh"
#include "shader_program/shader_program.hh"

namespace pogl
{
    class Engine
    {
    public:
        static Engine &instance();
        std::vector<std::shared_ptr<Renderable>> renderers;
        std::map<std::string, std::shared_ptr<ShaderProgram>> shaders;
        std::vector<std::shared_ptr<ShaderProgram>> camera_dependent_shaders;

        std::shared_ptr<Camera> camera;
        GLFWwindow *window;

        void display();
        void update();
        void init();

        Engine &add_renderer(std::shared_ptr<Renderable> renderer);

    private:
        bool _init_glfw();
        bool _init_glew();
        bool _init_GL();
        bool _init_shaders();
        bool _init_objects();
        bool _init_POV();

        Engine();
    };

} // namespace pogl