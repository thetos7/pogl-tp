#include <iostream>

#include "ground_object.hh"
#include "import/importer.hh"

namespace pogl
{
    using Self = GroundObject::Builder;

    Self::Builder()
        : _model_path("../resources/ground/model/ground.obj")
        , _shader()
        , _snow_mask_path("../resources/ground/textures/snow_mask")
        , _transform(Matrix4::identity())
        , _accumulation_rate(0.05)
    {}

    Self &Self::model(fs::path model_path)
    {
        _model_path = model_path;
        return *this;
    }
    Self &Self::shader(ShaderType shader)
    {
        _shader = shader;
        return *this;
    }
    Self &Self::mask(fs::path snow_mask_path)
    {
        _snow_mask_path = snow_mask_path;
        return *this;
    }
    Self &Self::transform(const Matrix4 &transform)
    {
        _transform = transform;
        return *this;
    }
    Self &Self::accumulation_rate(float accumulation_rate)
    {
        _accumulation_rate = accumulation_rate;
        return *this;
    }
    void Self::assert_integrity()
    {
        bool error = false;
        if (!_shader)
        {
            std::cerr << "ERROR: Cannot build ground, shader is missing.\n";
            error = true;
        }
        if (error)
        {
            throw std::logic_error("Cannot build ground object, see stderr.");
        }
    }

    std::optional<Self::BuildResult> Self::build()
    {
        assert_integrity();
        auto ground_buffers_result =
            Importer::read_file(_model_path)
                .configure_buffer("position", Importer::extract_position)
                .configure_buffer("uv", Importer::extract_texcoords)
                .configure_buffer("normal", Importer::extract_normals)
                .import();
        if (!ground_buffers_result)
        {
            std::cerr << "ERROR: could not import ground model, file is "
                         "missing ? (path: `"
                      << _model_path.c_str() << "`)\n";
            return std::nullopt;
        }
        auto snow_mask = FloatImageBuffer::load(_snow_mask_path);
        if (!snow_mask)
        {
            std::cerr << "ERROR: could not load ground snow mask, file is "
                         "missing ? (path: `"
                      << _snow_mask_path.c_str() << "`)\n";
            return std::nullopt;
        }
        auto ground_buffers = *ground_buffers_result;
        auto renderer = MeshRenderer::builder()
                            .shader(*_shader)
                            .add_buffer(ground_buffers.at("position"))
                            .add_attribute("vPosition", 3, 0)
                            .add_buffer(ground_buffers.at("uv"))
                            .add_attribute("vUV", 2, 1)
                            .add_buffer(ground_buffers.at("normal"))
                            .add_attribute("vNormal", 3, 2)
                            .transform(_transform)
                            .build();
        return std::make_shared<GroundObject>(
            renderer, *snow_mask,
            (*_shader)->get_texture_by_name("snow_height").value(),
            _accumulation_rate);
    }
} // namespace pogl
