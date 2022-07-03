#include "ground_object.hh"

#include <cmath>

namespace pogl
{
    GroundObject::Builder GroundObject::builder()
    {
        return Builder();
    }

    GroundObject::GroundObject(RendererType renderer,
                               const FloatImageBuffer &snow_mask,
                               std::shared_ptr<Texture> snow_height_texture,
                               float accumulation_rate)
        : _renderer(renderer)
        , _snow_mask(snow_mask)
        , _snow_height(FloatImageBuffer::sized(snow_mask.width(), snow_mask.height(),
                                          snow_mask.channels()))
        , _snow_height_texture(snow_height_texture)
        , _accumulation_rate(accumulation_rate)
    {}

    void GroundObject::draw()
    {
        _renderer->draw();
    }
    void GroundObject::update_camera(const Matrix4 &camera_transform)
    {
        _renderer->update_camera(camera_transform);
    }

    void GroundObject::update(double delta)
    {
        auto accumulation = delta * _accumulation_rate;
        for (int y = 0; y < _snow_height.height(); ++y)
        {
            for (int x = 0; x < _snow_height.width(); ++x)
            {
                auto height = _snow_height.at(y, x, 0);
                height += accumulation;
                height = std::min(height, _snow_mask.at(y, x, 0));
                _snow_height.at(y, x, 0) = height;
            }
        }

        _snow_height_texture->set_image(_snow_height, GL_RED, false);
    }
} // namespace pogl
