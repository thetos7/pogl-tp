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
    class MeshObjectRenderer : public Renderable
    {
    public:
        using ShaderType = std::shared_ptr<ShaderProgram>;
        using VaoType = GLuint;
        using BufferType = std::vector<GLfloat>;
        using DrawModeType = GLenum;
        using Self = MeshObjectRenderer;

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

            std::shared_ptr<MeshObjectRenderer> build();

        private:
            void assert_integrity();

            BufferCollectionType buffers_;
            std::optional<ShaderType> shader_;
            DrawModeType draw_mode_;
            AttributeConfigCollection attribute_config_;
        };

        MeshObjectRenderer(VaoType vao_id, DrawModeType draw_mode,
                           const ShaderType &shader,
                           size_t vertex_array_index_count);
        virtual ~MeshObjectRenderer() = default;

        static Builder builder();

        virtual void draw() override;
        virtual void update_camera(const Matrix4 &camera_transform) override;

    private:
        ShaderType shader_;
        VaoType vao_id_;
        DrawModeType draw_mode_;
        size_t vertex_array_index_count_;
    };

} // namespace pogl
