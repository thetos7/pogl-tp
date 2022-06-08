#include "shader_program.hh"

#include <GL/glew.h>
#include <filesystem>
#include <fstream>

namespace pogl
{
    using Self = ShaderProgram;

    ShaderProgram::ShaderProgram(const std::string &vertex_src,
                                 const std::string &fragment_src, bool ready)
        : _vertexSrc(vertex_src)
        , _fragSrc(fragment_src)
        , _ready(ready)
    {}

    ShaderProgram::ShaderProgram()
        : Self("", "", false)
    {}

    ShaderProgram::~ShaderProgram()
    {}

    std::string load_file(const fs::path &file_path)
    {
        auto file = std::ifstream(file_path);
        std::ostringstream content;
        std::string line;
        while (file >> line){
            content << line << '\n';
        }
        return content.str();
    }

    void ShaderProgram::compile_vertex()
    {
        auto shader_code = load_file(_vertexSrc);
    }

    void ShaderProgram::compile_fragment()
    {}

    void ShaderProgram::link_program()
    {}

    void ShaderProgram::post_compilation()
    {
        _ready = true;
    }

    Self ShaderProgram::make_program(const std::string &vertex_src,
                                     const std::string &fragment_src)
    {
        auto prog = Self(vertex_src, fragment_src, false);
        prog.compile_vertex();
        prog.compile_fragment();
        prog.link_program();
        prog.post_compilation();
        return prog;
    }

    char *ShaderProgram::get_log()
    {}

    bool ShaderProgram::is_ready() const
    {
        return _ready;
    }

    void ShaderProgram::activate()
    {}

} // namespace pogl
