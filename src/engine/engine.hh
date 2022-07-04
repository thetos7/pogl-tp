#pragma once
// glew first
#include <GL/glew.h>
// others
#include <GLFW/glfw3.h>
#include <cmath>
#include <map>
#include <memory>
#include <vector>

#include "camera/camera.hh"
#include "properties/renderable.hh"
#include "properties/updateable.hh"
#include "shader_program/shader_program.hh"
#include "texture/texture.hh"

namespace pogl
{
    class Engine
    {
    public:
        static constexpr float DEFAULT_FOV = M_PI_2;
        static constexpr float DEFAULT_ZNEAR = 0.5;
        static constexpr float DEFAULT_ZFAR = 100.;
        static constexpr float DEFAULT_ASPECT_RATIO = 1.;

        static Engine &instance();
        std::vector<std::shared_ptr<Renderable>> renderers;
        std::map<std::string, std::shared_ptr<ShaderProgram>> shaders;
        std::vector<Uniform> view_transform_uniforms;
        std::vector<Uniform> projection_uniforms;
        std::vector<std::shared_ptr<Updateable>> dynamic_objects;
        std::map<std::string, std::shared_ptr<Texture>> textures;

        std::shared_ptr<Camera> main_camera;
        GLFWwindow *window;

        void display();
        void update();
        void init();

        Engine &add_renderer(std::shared_ptr<Renderable> renderer);
        Engine &add_dynamic(std::shared_ptr<Updateable> object);
        Engine &add_texture(std::string name, std::shared_ptr<Texture> texture);

        void update_perspective(float aspect_ratio);

    private:
        bool _init_glfw();
        bool _init_glew();
        bool _init_GL();
        bool _init_shaders();
        bool _init_objects();
        bool _init_POV();
        bool _init_textures();
        void _init_camera_dependent_shader_map();

        Engine();
    };

} // namespace pogl
