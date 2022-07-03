#include "texture.hh"

#include "utils/gl_check.hh"

namespace pogl
{
    Texture::Texture()
        : Texture(0, GL_TEXTURE_2D, GL_RGB)
    {}

    Texture::Texture(GLuint texture_id, GLenum target, GLenum format)
        : _texture_id(texture_id)
        , _target(target)
        , _format(format)
    {}

    Texture::~Texture()
    {
        glDeleteTextures(1, &_texture_id);
        CHECK_GL_ERROR();
    }

    void Texture::use()
    {
        glBindTexture(_target, _texture_id);
        CHECK_GL_ERROR();
    }

    void Texture::image(const ImageBuffer &buffer, GLenum src_format)
    {
        glActiveTexture(GL_TEXTURE0);
        CHECK_GL_ERROR();
        use();

        glTexImage2D(_target, 0, _format, buffer.width(), buffer.height(), 0,
                     src_format, GL_UNSIGNED_BYTE, buffer.data());
        CHECK_GL_ERROR();
        glGenerateMipmap(_target);
        CHECK_GL_ERROR();
    }

    Texture::Builder Texture::builder()
    {
        return Builder();
    }
} // namespace pogl
