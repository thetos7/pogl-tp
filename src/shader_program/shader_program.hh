#pragma once

#include <string>
#include <filesystem>

namespace pogl
{
    namespace fs = std::filesystem;

    class ShaderProgram
    {
    public:
        using Self = ShaderProgram;

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
        static Self make_program(const std::string &vertex_src,
                                 const std::string &fragment_src);
        /**
         * @brief Get the compilation log
         *
         * @return char*
         */
        char *get_log();

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

    private:
        void compile_vertex();
        void compile_fragment();
        void link_program();
        void post_compilation();

        fs::path _vertexSrc;
        fs::path _fragSrc;
        bool _ready;
    };

} // namespace pogl
