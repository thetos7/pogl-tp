#pragma once

#include <GL/glew.h>
#include <filesystem>
#include <string>

namespace pogl
{
    namespace fs = std::filesystem;

    class ShaderProgram
    {
    public:
        using Self = ShaderProgram;
        using ShaderIdType = GLuint;
        using ProgramIdType = GLuint;

        ShaderProgram(const std::string &vertex_src,
                      const std::string &fragment_src, bool ready);
        ShaderProgram();
        ~ShaderProgram();

        /**
         * @brief Creates and compiles a new program from the contents of the
         * files at vertex_src and fragment_src.
         *
         * @param vertex_src Path to the vertex shader source file
         * @param fragment_src Path to the fragment shader source file
         * @return Self
         */
        static std::unique_ptr<Self>
        make_program(const std::string &vertex_src,
                     const std::string &fragment_src);
        /**
         * @brief Get the compilation log
         *
         * @return string& reference to the log
         */
        std::string &get_log();
        /**
         * @brief Get the compilation log
         *
         * @return const string&  constant reference to the log
         */
        const std::string &get_log() const;

        /**
         * @brief Check that the current program is ready to use (compiled,
         * linked, etc)
         *
         * @return true
         * @return false
         */
        bool is_ready() const;

        /**
         * @brief Use this program for rendering
         *
         */
        void activate();

        /**
         * @brief Alias for `activate`
         */
        inline void use()
        {
            activate();
        }

        /**
         * @brief Get the wrapped program id
         * 
         * @return ProgramIdType 
         */
        ProgramIdType get_program();

    private:
        bool compile_vertex();
        bool compile_fragment();
        bool link_program();
        bool post_compilation();

        fs::path _vertexSrc;
        fs::path _fragSrc;
        bool _ready;

        ShaderIdType _vertex;
        ShaderIdType _fragment;
        ProgramIdType _program;
        std::string _compilation_log;
    };

} // namespace pogl
