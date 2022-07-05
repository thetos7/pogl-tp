#include "image/stb_image.h"
#include "texture.hh"
#include "utils/gl_check.hh"
#include "utils/log.hh"
namespace pogl
{
    using Self = Texture::Builder;

    Self::Builder()
        : _texture_buffer(std::nullopt)
        , _border_color(std::nullopt)
        , _s_wrap_mode(GL_REPEAT)
        , _t_wrap_mode(GL_REPEAT)
        , _min_filter_mode(GL_LINEAR_MIPMAP_LINEAR)
        , _filter_mode(GL_LINEAR)
        , _format(GL_RGBA)
        , _src_format(GL_RGBA)
        , _target(GL_TEXTURE_2D)
    {}

    Self &Self::buffer(const BufferVariantType &buffer)
    {
        _texture_buffer = buffer;
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
        if (!_texture_buffer)
        {
            std::cerr << LOG_ERROR
                      << "Texture builder misses a texture buffer.\n ";
            error = true;
        }
        else if (std::holds_alternative<RGBBuffersType>(*_texture_buffer))
        {
            auto &buffers = std::get<RGBBuffersType>(*_texture_buffer);
            auto width = buffers[0].width();
            auto height = buffers[0].height();
            auto channels = buffers[0].channels();
            for (size_t i = 1; i < buffers.size(); ++i)
            {
                auto &buf = buffers[i];
                if (buf.width() != width || buf.height() != height
                    || buf.channels() != channels)
                {
                    std::cerr << LOG_ERROR
                              << "Texture builder is asked to build an atlas "
                                 "from texture with inconsistent sizes.\n";
                    std::cerr << LOG_INFO << "First image has width = " << width
                              << ", height = " << height
                              << ", channels = " << channels
                              << ", image number " << i
                              << " has width = " << buf.width()
                              << ", height = " << buf.height()
                              << ", channels = " << buf.channels() << ".\n";
                    error = true;
                    break;
                }
            }
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

        if (std::holds_alternative<RGBBuffersType>(*_texture_buffer))
        {
            auto &buffers = std::get<RGBBuffersType>(*_texture_buffer);
            auto width = buffers[0].width();
            auto height = buffers[0].height();
            auto channels = buffers[0].channels();
            auto depth = buffers.size();
            auto pixel_data_count = width * height * channels;
            auto bytes = std::vector<GLubyte>(pixel_data_count * depth);
            auto it = bytes.begin();
            for (size_t i = 0; i < buffers.size(); ++i)
            {
                const auto &buf = buffers[i];
                std::copy(buf.pixels().begin(), buf.pixels().end(),
                          bytes.begin() + i * pixel_data_count);
            }
            glTexImage3D(_target, 0, _format, width, height, depth, 0,
                         _src_format, GL_UNSIGNED_BYTE, bytes.data());
            CHECK_GL_ERROR();
        }
        else if (std::holds_alternative<RGBImageBuffer>(*_texture_buffer))
        {
            auto &buffer = std::get<RGBImageBuffer>(*_texture_buffer);
            glTexImage2D(_target, 0, _format, buffer.width(), buffer.height(),
                         0, _src_format, GL_UNSIGNED_BYTE, buffer.data());
            CHECK_GL_ERROR();
        }
        else
        {
            auto &buffer = std::get<FloatImageBuffer>(*_texture_buffer);
            glTexImage2D(_target, 0, _format, buffer.width(), buffer.height(),
                         0, _src_format, GL_FLOAT, buffer.data());
            CHECK_GL_ERROR();
        }
        glGenerateMipmap(_target);
        CHECK_GL_ERROR();

        return std::make_shared<Texture>(texture_id, _target, _format);
    }
} // namespace pogl
