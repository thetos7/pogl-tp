#include "image/stb_image.h"
#include "texture.hh"
#include "utils/gl_check.hh"

namespace pogl
{
    using Self = Texture::Builder;

    Self::Builder()
        : _texture_path(std::nullopt)
        , _border_color(std::nullopt)
        , _s_wrap_mode(GL_REPEAT)
        , _t_wrap_mode(GL_REPEAT)
        , _min_filter_mode(GL_LINEAR_MIPMAP_LINEAR)
        , _filter_mode(GL_LINEAR)
        , _format(GL_RGBA)
        , _src_format(GL_RGBA)
        , _target(GL_TEXTURE_2D)
    {}

    Self &Self::path(const fs::path &path)
    {
        _texture_path = path;
        return *this;
    }

    Self &Self::border(const Vector4 &color)
    {
        _border_color = color;
        return *this;
    }

    Self &Self::s_wrap(GLenum mode)
    {
        _s_wrap_mode = mode;
        return *this;
    }

    Self &Self::t_wrap(GLenum mode)
    {
        _t_wrap_mode = mode;
        return *this;
    }

    Self &Self::wrap(GLenum mode)
    {
        return s_wrap(mode).t_wrap(mode);
    }

    Self &Self::min_filter(GLenum mode)
    {
        _min_filter_mode = mode;
        return *this;
    }

    Self &Self::filter(GLenum mode)
    {
        _filter_mode = mode;
        return *this;
    }

    Self &Self::format(GLenum format)
    {
        _format = format;
        return *this;
    }

    Self &Self::src_format(GLenum format)
    {
        _src_format = format;
        return *this;
    }

    Self &Self::target(GLenum target)
    {
        _target = target;
        return *this;
    }

    void Self::assert_integrity()
    {
        bool error = false;
        if (!_texture_path)
        {
            std::cerr << "ERROR: Texture builder misses an image path.\n";
            error = true;
        }
        if (error)
        {
            throw std::logic_error(
                "builder integrity check failed, see stderr.");
        }
    }

    std::shared_ptr<Texture> Self::build()
    {
        assert_integrity();

        glActiveTexture(GL_TEXTURE0);
        CHECK_GL_ERROR();

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        CHECK_GL_ERROR();
        glBindTexture(_target, texture_id);
        CHECK_GL_ERROR();
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, _s_wrap_mode);
        CHECK_GL_ERROR();
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, _t_wrap_mode);
        CHECK_GL_ERROR();
        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, _min_filter_mode);
        CHECK_GL_ERROR();
        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, _filter_mode);
        CHECK_GL_ERROR();

        glTexParameterfv(
            _target, GL_TEXTURE_BORDER_COLOR,
            _border_color.value_or(DEFAULT_BORDER_COLOR).as_vec().data());
        CHECK_GL_ERROR();

        int width = 0;
        int height = 0;
        int channel_count = 0;
        auto data = stbi_load(_texture_path->c_str(), &width, &height,
                              &channel_count, 0);
        if (data == nullptr)
        {
            std::cerr << "ERROR: Could not load image at `" << *_texture_path
                      << "`.\n";
            throw std::logic_error("texture image load failed.");
        }

        glTexImage2D(_target, 0, _format, width, height, 0, _src_format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(_target);

        stbi_image_free(data);
        return std::make_shared<Texture>(texture_id, _target);
    }
} // namespace pogl
