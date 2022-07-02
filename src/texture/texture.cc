#include "texture.hh"

namespace pogl
{
    Texture::Texture()
        : Texture(0, GL_TEXTURE_2D)
    {}

    Texture::Texture(GLuint texture_id, GLenum target)
        : _texture_id(texture_id)
        , _target(target)
    {}

    Texture::~Texture()
    {
        glDeleteTextures(1, &_texture_id);
    }

    void Texture::use()
    {
        glBindTexture(GL_TEXTURE_2D, _texture_id);
    }

    Texture::Builder Texture::builder()
    {
        return Builder();
    }
} // namespace pogl
