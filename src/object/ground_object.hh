#pragma once

#include <filesystem>
#include <optional>

#include "image/image_buffer.hh"
#include "mesh_renderer.hh"
#include "properties/renderable.hh"
#include "properties/updateable.hh"

namespace pogl
{
    namespace fs = std::filesystem;

    class GroundObject
        : public Updateable
        , public Renderable
    {
    public:
        using RendererType = std::shared_ptr<MeshRenderer>;

        class Builder
        {
        public:
            using Self = Builder;
            using ShaderType = MeshRenderer::ShaderType;
            using BuildResult = std::shared_ptr<GroundObject>;
            Builder();

            Self &model(fs::path model_path);
            Self &shader(ShaderType shader);
            Self &mask(fs::path snow_mask_path);
            Self &transform(const Matrix4 &transform);
            Self &accumulation_rate(float accumulation_rate);

            std::optional<BuildResult> build();

        private:
            void assert_integrity();
            fs::path _model_path;
            std::optional<ShaderType> _shader;
            fs::path _snow_mask_path;
            Matrix4 _transform;
            float _accumulation_rate;
        };

        static Builder builder();

        GroundObject(RendererType renderer, const FloatImageBuffer &snow_mask,
                     std::shared_ptr<Texture> snow_height_texture,
                     float accumulation_rate);
        virtual ~GroundObject() = default;

        virtual void draw() override;
        virtual void update_camera(const Matrix4 &camera_transform) override;
        virtual void update(double delta) override;

    private:
        RendererType _renderer;
        FloatImageBuffer _snow_mask;
        FloatImageBuffer _snow_height;
        std::shared_ptr<Texture> _snow_height_texture;
        float _accumulation_rate;
    };

} // namespace pogl
