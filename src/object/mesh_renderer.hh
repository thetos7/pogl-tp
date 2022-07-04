#pragma once

#include <GL/glew.h>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "properties/renderable.hh"
#include "shader_program/shader_program.hh"

namespace pogl
{
    class MeshRenderer : public Renderable
    {
    public:
        using ShaderType = std::shared_ptr<ShaderProgram>;
        using VaoType = GLuint;
        using BufferType = std::vector<GLfloat>;
        using DrawModeType = GLenum;
        using UniformType = std::optional<Uniform>;
        using Self = MeshRenderer;

        class Builder
        {
        public:
            using AttributeConfigType = std::tuple<std::string, int>;
            using AttributeConfigCollection =
                std::map<size_t, std::vector<AttributeConfigType>>;
            using BufferCollectionType = std::vector<BufferType>;

            Builder();

            Builder &add_buffer(const BufferType &buffer);
            Builder &shader(const ShaderType &shader);
            Builder &draw_mode(DrawModeType draw_mode);
            Builder &add_attribute(std::string name, int size,
                                   size_t buffer_id = 0);
            Builder &transform(Matrix4 transform);

            std::shared_ptr<MeshRenderer> build();

        private:
            void assert_integrity();

            BufferCollectionType _buffers;
            std::optional<ShaderType> _shader;
            DrawModeType _draw_mode;
            AttributeConfigCollection _attribute_config;
            Matrix4 _transform;
        };

        MeshRenderer(VaoType vao_id, DrawModeType draw_mode,
                     const ShaderType &shader, size_t vertex_array_index_count,
                     std::vector<GLuint> _buffer_ids, const Matrix4 &transform,
                     UniformType transform_uniform);
        virtual ~MeshRenderer();

        static Builder builder();

        virtual void draw() override;

        void set_transform(const Matrix4 &transform);

        ShaderType shader();

    private:
        ShaderType _shader;
        VaoType _vao_id;
        DrawModeType _draw_mode;
        size_t _vertex_array_index_count;
        std::vector<GLuint> _buffer_ids;
        Matrix4 _transform;
        UniformType _transform_uniform;
    };

} // namespace pogl
