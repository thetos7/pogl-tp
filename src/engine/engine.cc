#include "engine.hh"

#include <cmath>
#include <optional>

#include "app/callbacks.hh"
#include "app/input.hh"
#include "import/importer.hh"
#include "inputstate/inputstate.hh"
#include "object/ground_object.hh"
#include "object/mesh_renderer.hh"
#include "particle_system/particle_system.hh"
#include "utils/definitions.hh"
#include "utils/gl_check.hh"
#include "utils/log.hh"

#define DEFAULT_SCENE 0

#if DEFAULT_SCENE
// enables color
#    define COLOR 1
#    include "vertex_data.hh"
#endif // DEFAULT_SCENE

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
        , view_transform_uniforms()
        , projection_uniforms()
        , dynamic_objects()
        , main_camera(nullptr)
        , window(nullptr)
    {}

    bool Engine::_init_glfw()
    {
        if (!glfwInit())
        {
            std::cerr << LOG_ERROR << "GLFW initialisation failed.\n";
            return false;
        }
        glfwSetErrorCallback(on_error);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        auto &window = Engine::instance().window;
        window = glfwCreateWindow(700, 700, "Test OpenGl - POGL (GLFW)",
                                  nullptr, nullptr);
        if (!window)
        {
            std::cerr << LOG_ERROR << "GLFW Window creation failed.\n";
            return false;
        }
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, on_key_update);
        glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);
        glfwSetWindowFocusCallback(window, on_focus);

        update_cursor_capture(window, get_input_state());

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
        glEnable(GL_BLEND);
        CHECK_GL_ERROR();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        CHECK_GL_ERROR();
        glClearColor(0.800, 0.839, 0.902, 1.0); // "sky" color
        CHECK_GL_ERROR();
        glPixelStorei(GL_PACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
        CHECK_GL_ERROR();
        glPixelStorei(GL_UNPACK_ALIGNMENT, PIXEL_BYTE_ALIGNEMENT_LEN);
        CHECK_GL_ERROR();

        return true;
    }

    bool Engine::_init_shaders()
    {
#if DEFAULT_SCENE
#    if COLOR
        auto cube_shader = ShaderProgram::make_program(
            "../resources/shaders/vert_color/vertex.glsl",
            "../resources/shaders/vert_color/fragment.glsl");
#    else
        auto cube_shader = ShaderProgram::make_program(
            "../resources/shaders/uniform/vertex.glsl",
            "../resources/shaders/uniform/fragment.glsl");
#    endif // COLOR

        shaders.emplace("cube_shader", cube_shader);

        auto color = cube_shader->uniform("obj_color");
        if (color)
        {
            color->set_vec4(1., 0., 0., 1.);
        }

        auto plane_shader = ShaderProgram::make_program(
            "../resources/shaders/textured/vertex.glsl",
            "../resources/shaders/textured/fragment.glsl");
        plane_shader->set_unit_name("texture", 0);

        shaders.emplace("plane_shader", plane_shader);
#endif // DEFAULT_SCENE
        auto uv_debug_shader = ShaderProgram::make_program(
            "../resources/shaders/uv_debug/vertex.glsl",
            "../resources/shaders/uv_debug/fragment.glsl");

        shaders.emplace("uv_debug", uv_debug_shader);

        // <ground shader>
        auto ground_shader = ShaderProgram::make_program(
            "../resources/ground/shader/vertex.glsl",
            "../resources/ground/shader/fragment.glsl");
        {
            // texture unit names
            ground_shader->set_unit_name("snow_height", 0);
            ground_shader->set_unit_name("under_texture", 1);
            ground_shader->set_unit_name("snow_texture", 2);
            // set up uniforms
            auto up_u = ground_shader->uniform("up");
            if (up_u)
                up_u->set_vec3(Vector3::up());
            // sun lighting
            auto sun_direction_u = ground_shader->uniform("sun_direction");
            if (sun_direction_u)
                sun_direction_u->set_vec3(-Vector3::up());
            auto sun_intensity_u = ground_shader->uniform("sun_intensity");
            if (sun_intensity_u)
                sun_intensity_u->set_float(1.0);
            auto sun_color_u = ground_shader->uniform("sun_color");
            if (sun_color_u)
                sun_color_u->set_vec3(Vector3::all(1.0));
            // ambient lighting
            auto ambient_color_u = ground_shader->uniform("ambient_color");
            if (ambient_color_u)
                ambient_color_u->set_vec3(Vector3::all(1.0));
            auto ambient_intensity_u =
                ground_shader->uniform("ambient_intensity");
            if (ambient_intensity_u)
                ambient_intensity_u->set_float(0.2);
            // set texture sampler unit
            auto snow_height_u = ground_shader->uniform("snow_height");
            if (snow_height_u)
                snow_height_u->set_int(0);
            auto under_texture_u = ground_shader->uniform("under_texture");
            if (under_texture_u)
                under_texture_u->set_int(1);
            auto snow_texture_u = ground_shader->uniform("snow_texture");
            if (snow_texture_u)
                snow_texture_u->set_int(2);
            auto scale_u = ground_shader->uniform("scale");
            // displacement scale
            if (scale_u)
                scale_u->set_float(0.5);
        }
        shaders.emplace("ground", ground_shader);
        // </ground shader>

        auto particles_shader = ShaderProgram::make_program(
            "../resources/shaders/particle_system/vertex.glsl",
            "../resources/shaders/particle_system/fragment.glsl");
        shaders.emplace("particle_system", particles_shader);

        _init_camera_dependent_shader_map();
        return true;
    }

    void Engine::_init_camera_dependent_shader_map()
    {
        // find shaders which require the camera transform, i.e. that have a
        // mat4 view_transform uniform & a mat4 projection_matrix uniform
        for (auto [name, s] : shaders)
        {
            const auto view_transform_uniform =
                s->uniform(definitions::VIEW_TRANSFORM_UNIFORM_NAME);
            const auto projection_uniform =
                s->uniform(definitions::PROJECTION_UNIFORM_NAME);

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
                std::cerr << LOG_WARNING << "shader program `" << name
                          << "` has one of "
                          << definitions::VIEW_TRANSFORM_UNIFORM_NAME << " or "
                          << definitions::PROJECTION_UNIFORM_NAME
                          << " "
                             "uniforms, but not both.\n";
                error = true;
            }

            if (has_projection_uniform && !projection_uniform_type_ok)
            {
                std::cerr << LOG_WARNING << "shader program `" << name
                          << "` has a `" << definitions::PROJECTION_UNIFORM_NAME
                          << "` uniform which's type "
                             "is not mat4.\n";
                error = true;
            }

            if (has_view_transform_uniform && !view_transform_uniform_type_ok)
            {
                std::cerr << LOG_WARNING << "shader program `" << name
                          << "` has a `"
                          << definitions::VIEW_TRANSFORM_UNIFORM_NAME
                          << "` uniform which's "
                             "type is not mat4.\n";
                error = true;
            }

            if (!error)
            {
                view_transform_uniforms.push_back(
                    s->uniform(definitions::VIEW_TRANSFORM_UNIFORM_NAME)
                        .value());
                projection_uniforms.push_back(
                    s->uniform(definitions::PROJECTION_UNIFORM_NAME).value());
            }
        }
    }

    bool Engine::_init_textures()
    {
#if DEFAULT_SCENE
        auto smiley_tex =
            Texture::builder()
                .buffer(RGBImageBuffer::load("../resources/textures/smiley.png")
                            .value())
                .border(Vector4(0, 0, 0, 0))
                .wrap(GL_CLAMP_TO_BORDER)
                .src_format(GL_RGBA)
                .format(GL_RGBA)
                .build();
        shaders["plane_shader"]->set_texture("texture", smiley_tex);
        this->add_texture("smiley", smiley_tex);
#endif // DEFAULT_SCENE

        auto ground_shader = shaders["ground"];
        auto rock_tex =
            Texture::builder()
                .buffer(RGBImageBuffer::load(
                            "../resources/ground/textures/rock_texture.png", 3)
                            .value())
                .wrap(GL_REPEAT)
                .src_format(GL_RGB)
                .format(GL_RGB)
                .build();
        ground_shader->set_texture("under_texture", rock_tex);
        this->add_texture("rock_texture", rock_tex);

        auto snow_tex =
            Texture::builder()
                .buffer(RGBImageBuffer::load(
                            "../resources/ground/textures/snow_texture.png", 3)
                            .value())
                .wrap(GL_REPEAT)
                .src_format(GL_RGB)
                .format(GL_RGB)
                .build();
        ground_shader->set_texture("snow_texture", snow_tex);
        this->add_texture("snow_texture", snow_tex);

        auto snow_height_tex =
            Texture::builder()
                .buffer(FloatImageBuffer::sized(1024, 1024, 4))
                .wrap(GL_CLAMP_TO_BORDER)
                .border(Vector4(0, 0, 0, 1))
                .src_format(GL_RED)
                .format(GL_R32F)
                .min_filter(GL_LINEAR)
                .build();
        ground_shader->set_texture("snow_height", snow_height_tex);
        this->add_texture("snow_height", snow_height_tex);
        return true;
    }

    bool Engine::_init_objects()
    {
#if DEFAULT_SCENE
        auto cube_renderer = MeshRenderer::builder()
                                 .shader(shaders["cube_shader"])
                                 .add_buffer(cube_vertex_position_data)
                                 .add_attribute("vPosition", 3, 0)
#    if COLOR
                                 .add_buffer(cube_vertex_color_data)
                                 .add_attribute("vColor", 3, 1)
#    endif
                                 .build();
        this->add_renderer(cube_renderer);
#endif // DEFAULT_SCENE

        auto ground_option =
            GroundObject::builder()
                .shader(shaders["ground"])
                .mask("../resources/ground/textures/snow_mask.png")
                .model("../resources/ground/model/ground.obj")
                .accumulation_rate(0.01)
                .transform(Matrix4::translation(0, 0, -1))
                .build();
        if (!ground_option)
        {
            std::cerr << LOG_ERROR << "ground could not be built.\n";
        }
        else
        {
            auto ground = *ground_option;
            this->add_renderer(ground);
            this->add_dynamic(ground);
        }

        std::shared_ptr<ParticleSystem> particle_sys = std::make_shared<ParticleSystem>(shaders["particle_system"]);
        this->add_renderer(particle_sys);
        this->add_dynamic(particle_sys);

#if DEFAULT_SCENE
        auto plane_renderer = MeshRenderer::builder()
                                  .shader(shaders["plane_shader"])
                                  .add_buffer(plane_vertex_position_data)
                                  .add_attribute("vPosition", 3, 0)
                                  .add_buffer(plane_uv_data)
                                  .add_attribute("vUV", 2, 1)
                                  .build();
        this->add_renderer(plane_renderer);
#endif // DEFAULT_SCENE
        return true;
    }

    bool Engine::_init_POV()
    {
        const auto projection_matrix = Matrix4::perspective(
            DEFAULT_FOV, DEFAULT_ASPECT_RATIO, DEFAULT_ZNEAR, DEFAULT_ZFAR);
        auto camera = std::make_shared<Camera>(Vector3(3.5, 0, 0), 0.0, M_PI,
                                               projection_matrix);

        this->add_dynamic(camera);

        main_camera = camera;

        const auto view_transform = main_camera->get_transform();

        for (size_t i = 0; i < projection_uniforms.size(); ++i)
        {
            view_transform_uniforms[i].set_mat4(view_transform);
            projection_uniforms[i].set_mat4(projection_matrix);
        }
        return true;
    }

    void Engine::init()
    {
        std::cout << LOG_INFO << "initialising GLFW...\n";
        _init_glfw();
        std::cout << LOG_INFO << "initialising GLEW...\n";
        _init_glew();
        std::cout << LOG_INFO << "initialising GL...\n";
        _init_GL();
        std::cout << LOG_INFO << "initialising shaders...\n";
        _init_shaders();
        std::cout << LOG_INFO << "initialising textures...\n";
        _init_textures();
        std::cout << LOG_INFO << "initialising POV...\n";
        _init_POV();
        std::cout << LOG_INFO << "initialising objects...\n";
        _init_objects();

    }

    void Engine::display()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CHECK_GL_ERROR();
        const auto view_transform = main_camera->get_transform();
        for (auto view_transform_uniform : view_transform_uniforms)
        {
            view_transform_uniform.set_mat4(view_transform);
        }
        for (auto renderer : renderers)
        {
            renderer->draw();
        }
    }

    void Engine::update()
    {
        mouse_update();
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

    Engine &Engine::add_renderer(std::shared_ptr<Drawable> renderer)
    {
        renderers.push_back(renderer);
        return *this;
    }

    Engine &Engine::add_dynamic(std::shared_ptr<Updateable> object)
    {
        dynamic_objects.push_back(object);
        return *this;
    }

    Engine &Engine::add_texture(std::string name,
                                std::shared_ptr<Texture> texture)
    {
        textures.emplace(name, texture);
        return *this;
    }

    void Engine::update_perspective(float aspect_ratio)
    {
        const auto projection = Matrix4::perspective(
            DEFAULT_FOV, aspect_ratio, DEFAULT_ZNEAR, DEFAULT_ZFAR);
        main_camera->set_projection(projection);
        for (auto projection_uniform : projection_uniforms)
        {
            projection_uniform.set_mat4(projection);
        }
    }
} // namespace pogl
