#include "shader_program.hh"

#include <GL/glew.h>
#include <filesystem>
#include <fstream>

#include "utils/gl_check.hh"

namespace pogl
{
    using Self = ShaderProgram;

    ShaderProgram::ShaderProgram(const std::string &vertex_src,
                                 const std::string &fragment_src, bool ready)
        : _vertexSrc(vertex_src)
        , _fragSrc(fragment_src)
        , _ready(ready)
        , _vertex(0)
        , _fragment(0)
        , _program(0)
        , _compilation_log()
    {}

    ShaderProgram::ShaderProgram()
        : Self("", "", false)
    {}

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(_program);
        CHECK_GL_ERROR();
    }

    std::string load_file(const fs::path &file_path)
    {
        auto file = std::ifstream(file_path);
        std::ostringstream content;
        std::string line;
        while (std::getline(file, line))
        {
            content << line << '\n';
        }
        return content.str();
    }

    inline bool check_gl_compilation(ShaderProgram::ShaderIdType &shader,
                                     std::string &compilation_log,
                                     const char *shader_name,
                                     const fs::path &program_path)
    {
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE)
        {
            GLint log_size = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
            std::string log_text(log_size, '\0');
            glGetShaderInfoLog(shader, log_size, NULL, log_text.data());

            std::ostringstream oss;
            oss << "\e[31m[SHADER COMPILE ERROR]\e[39m\n"
                   "Failed to compile shader `"
                << shader_name << "` in `" << program_path
                << "`\n"
                   "Compilation log:\n"
                   "\n";
            oss << log_text << "\n\n";

            const auto str = oss.str();
            std::cerr << str;
            compilation_log += str;

            glDeleteShader(shader);
            CHECK_GL_ERROR();
            shader = 0;
            return false;
        }
        return true;
    }

    bool ShaderProgram::compile_vertex()
    {
        // vertex shader
        _vertex = glCreateShader(GL_VERTEX_SHADER);
        CHECK_GL_ERROR();

        const auto vertex_shader_code = load_file(_vertexSrc);
        const char *vertex_sources[] = { vertex_shader_code.c_str() };
        glShaderSource(_vertex, 1, vertex_sources, NULL);
        CHECK_GL_ERROR();

        glCompileShader(_vertex);
        return check_gl_compilation(_vertex, _compilation_log, "VERTEX",
                                    _vertexSrc);
    }

    bool ShaderProgram::compile_fragment()
    {
        _fragment = glCreateShader(GL_FRAGMENT_SHADER);
        CHECK_GL_ERROR();

        const auto fragment_shader_code = load_file(_fragSrc);
        const char *fragment_sources[] = { fragment_shader_code.c_str() };
        glShaderSource(_fragment, 1, fragment_sources, NULL);
        CHECK_GL_ERROR();

        glCompileShader(_fragment);
        return check_gl_compilation(_fragment, _compilation_log, "FRAGMENT",
                                    _fragSrc);
    }

    bool ShaderProgram::link_program()
    {
        _program = glCreateProgram();
        CHECK_GL_ERROR();

        glAttachShader(_program, _vertex);
        CHECK_GL_ERROR();
        glAttachShader(_program, _fragment);
        CHECK_GL_ERROR();

        glLinkProgram(_program);
        // check linking success
        GLint is_linked = 0;
        glGetProgramiv(_program, GL_LINK_STATUS, &is_linked);
        if (is_linked != GL_TRUE)
        {
            GLint log_size = 0;
            glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &log_size);

            std::string linking_log(log_size, '\0');
            glGetProgramInfoLog(_program, log_size, NULL, linking_log.data());

            std::ostringstream oss;
            oss << "\e[31m[LINKING ERROR]\e[39m\n"
                   "Program linking failed\n"
                   "Linking log:\n\n"
                << linking_log << "\n\n";
            const auto msg = oss.str();
            std::cerr << msg << std::endl;
            _compilation_log += msg;

            glDeleteProgram(_program);
            CHECK_GL_ERROR();
            glDeleteShader(_fragment);
            CHECK_GL_ERROR();
            glDeleteShader(_vertex);
            CHECK_GL_ERROR();
            return false;
        }

        glDetachShader(_program, _vertex);
        CHECK_GL_ERROR();
        glDetachShader(_program, _fragment);
        CHECK_GL_ERROR();
        return true;
    }

    bool ShaderProgram::post_compilation()
    {
        bool errored = false;
        glDeleteShader(_vertex);
        errored = errored || CHECK_GL_ERROR();
        glDeleteShader(_fragment);
        errored = errored || CHECK_GL_ERROR();

        _ready = !errored;
        return !errored;
    }

    std::unique_ptr<Self> ShaderProgram::make_program(const std::string &vertex_src,
                                     const std::string &fragment_src)
    {
        auto prog = std::make_unique<Self>(vertex_src, fragment_src, false);
        const auto vert_compiled = prog->compile_vertex();
        const auto frag_compiled = prog->compile_fragment();

        if (!vert_compiled || !frag_compiled)
        {
            constexpr auto msg =
                "One of the shader program's compilation failed";
            std::cerr << msg << std::endl;
            throw std::runtime_error(msg);
        }

        const auto linked = prog->link_program();
        if (!linked)
        {
            constexpr auto msg = "Shader program linking failed";
            std::cerr << msg << std::endl;
            throw std::runtime_error(msg);
        }
        const auto post = prog->post_compilation();
        if (!post)
        {
            constexpr auto msg = "Error in post compilation";
            std::cerr << msg << std::endl;
            throw std::runtime_error(msg);
        }
        return prog;
    }

    std::string &ShaderProgram::get_log()
    {
        return _compilation_log;
    }

    const std::string &ShaderProgram::get_log() const
    {
        return _compilation_log;
    }

    bool ShaderProgram::is_ready() const
    {
        return _ready;
    }

    void ShaderProgram::activate()
    {
        glUseProgram(_program);
        CHECK_GL_ERROR();
    }

    ShaderProgram::ProgramIdType ShaderProgram::get_program()
    {
        return _program;
    }

} // namespace pogl
