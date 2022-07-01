#include "engine.hh"

#include <cmath>
#include <optional>

#include "app/callbacks.hh"
#include "app/input.hh"
#include "inputstate/inputstate.hh"
#include "object/mesh_object_renderer.hh"
#include "utils/gl_check.hh"

// enables color
#define COLOR 1
#include "vertex_data.hh"

namespace pogl
{

    Engine &Engine::instance()
    {
        static std::optional<Engine> instance_;
        if (!instance_)
        {
            instance_ = Engine();
        }
        return *instance_;
    }
    Engine::Engine()
        : renderers()
        , shaders()
        , camera_dependent_shaders()
        , dynamic_objects()
        , main_camera(nullptr)
        , window(nullptr)
    {}

    bool Engine::_init_glfw()
    {
        if (!glfwInit())
        {
            std::cerr << "ERROR: GLFW initialisation failed.\n";
            return false;
        }
        glfwSetErrorCallback(on_error);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        auto &window = Engine::instance().window;
        window = glfwCreateWindow(700, 700, "Test OpenGl - POGL (GLFW)",
                                  nullptr, nullptr);
        if (!window)
        {
            std::cerr << "ERROR: GLFW Window creation failed.\n";
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, on_key_update);
        glfwSetWindowSizeCallback(window, on_window_resize);
        glfwSetWindowFocusCallback(window, on_focus);

        update_cursor_capture(window, get_input_state());

        glfwSetCursorPosCallback(window, on_mouse_move);

        return true;
    }

    bool Engine::_init_glew()
    {
        return (glewInit() == GLEW_OK);
    }

    bool Engine::_init_GL()
    {
        constexpr int PIXEL_BYTE_ALIGNEMENT_LEN = 1; // 1 byte for 8-bit color
        glEnable(GL_DEPTH_TEST);
        CHECK_GL_ERROR();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        CHECK_GL_ERROR();
        glEnable(GL_CULL_FACE);
        CHECK_GL_ERROR();
        glClearColor(0.5, 0.5, 0.5, 1.0);
        CHECK_GL_ERROR();
        glPixelStorei(GL_PACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
        CHECK_GL_ERROR();
        glPixelStorei(GL_UNPACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
        CHECK_GL_ERROR();

        return true;
    }

    bool Engine::_init_shaders()
    {
#if COLOR
        auto shader = ShaderProgram::make_program(
            "../resources/shaders/vert_color/vertex.glsl",
            "../resources/shaders/vert_color/fragment.glsl");
#else
        auto shader = ShaderProgram::make_program(
            "../resources/shaders/uniform/vertex.glsl",
            "../resources/shaders/uniform/fragment.glsl");
#endif // COLOR

        shaders.emplace("mesh_shader", shader);

        auto color = shader->uniform("obj_color");
        if (color)
        {
            color->set_vec4(1., 0., 0., 1.);
        }

        // find shaders which require the camera transform, i.e. that have a
        // mat4 view_transform_matrix uniform & a mat4 projection_matrix uniform
        for (auto [name, s] : shaders)
        {
            const auto view_transform_uniform =
                s->uniform("view_transform_matrix");
            const auto projection_uniform = s->uniform("projection_matrix");

            bool has_view_transform_uniform =
                view_transform_uniform.has_value();
            bool view_transform_uniform_type_ok = has_view_transform_uniform
                && view_transform_uniform->type() == GL_FLOAT_MAT4;

            bool has_projection_uniform = projection_uniform.has_value();
            bool projection_uniform_type_ok = has_projection_uniform
                && projection_uniform->type() == GL_FLOAT_MAT4;

            bool error = false;

            if (has_view_transform_uniform != has_projection_uniform)
            {
                std::cerr
                    << "WARNING: shader program `" << name
                    << "` has one of view_tranform_matrix or projection_matrix "
                       "uniforms, but not both.\n";
                error = true;
            }

            if (has_projection_uniform && !projection_uniform_type_ok)
            {
                std::cerr << "WARNING: shader program `" << name
                          << "` has a `projection_matrix` uniform which's type "
                             "is not mat4.\n";
                error = true;
            }

            if (has_view_transform_uniform && !view_transform_uniform_type_ok)
            {
                std::cerr << "WARNING: shader program `" << name
                          << "` has a `view_transform_matrix` uniform which's "
                             "type is not mat4.\n";
                error = true;
            }

            if (has_view_transform_uniform && view_transform_uniform_type_ok
                && has_projection_uniform && projection_uniform_type_ok)
            {
                camera_dependent_shaders.push_back(s);
            }
        }
        return true;
    }

    bool Engine::_init_objects()
    {
        auto mesh_renderer = MeshObjectRenderer::builder()
                                 .shader(shaders["mesh_shader"])
                                 .add_buffer(vertex_position_data)
                                 .add_attribute("vPosition", 3, 0)
#if COLOR
                                 .add_buffer(vertex_color_data)
                                 .add_attribute("vColor", 3, 1)
#endif
                                 .build();

        this->add_renderer(mesh_renderer);
        return true;
    }

    bool Engine::_init_POV()
    {
        const auto projection_matrix =
            Matrix4::frustum(-0.5, 0.5, -0.5, 0.5, 1, 100);
        auto camera = std::make_shared<Camera>(Vector3(3.5, 0, 0), 0.0, M_PI,
                                               projection_matrix);

        this->add_dynamic(camera);

        main_camera = camera;

        const auto view_transform = main_camera->get_transform();
        std::cerr << main_camera->get_forward() << "\n";

        for (auto s : camera_dependent_shaders)
        {
            s->uniform("view_transform_matrix")->set_mat4(view_transform);
            s->uniform("projection_matrix")->set_mat4(projection_matrix);
        }
        return true;
    }

    void Engine::init()
    {
        std::cout << "initialising GLFW...\n";
        _init_glfw();
        std::cout << "initialising GLEW...\n";
        _init_glew();
        std::cout << "initialising GL...\n";
        _init_GL();
        std::cout << "initialising shaders...\n";
        _init_shaders();
        std::cout << "initialising objects...\n";
        _init_objects();
        std::cout << "initialising POV...\n";
        _init_POV();
    }

    void Engine::display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CHECK_GL_ERROR();
        const auto view_transform = main_camera->get_transform();
        for (auto shader : camera_dependent_shaders) {
            shader->uniform("view_transform_matrix")->set_mat4(view_transform);
        }
        for (auto renderer : renderers)
        {
            renderer->draw();
        }
    }

    Engine &Engine::add_renderer(std::shared_ptr<Renderable> renderer)
    {
        renderers.push_back(renderer);
        return *this;
    }

    Engine &Engine::add_dynamic(std::shared_ptr<Updateable> object)
    {
        dynamic_objects.push_back(object);
        return *this;
    }

    void Engine::update()
    {
        static double last_tick = 0;
        const double now = glfwGetTime();
        const double delta = now - last_tick;
        last_tick = now;

        // update objects
        for (auto object : dynamic_objects)
        {
            object->update(delta);
        }

        auto &input_state = get_input_state();
        input_state.mouse_x_axis = 0;
        input_state.mouse_y_axis = 0;
    }
} // namespace pogl
