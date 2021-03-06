#pragma once

#include <GL/glew.h>
#include <filesystem>
#include <memory>
#include <optional>
#include <variant>

#include "image/image_buffer.hh"
#include "vector4/vector4.hh"

namespace pogl
{
    namespace fs = std::filesystem;
    class Texture
    {
    public:
        using RGBBuffersType = std::vector<RGBImageBuffer>;
        using BufferVariantType = std::optional<std::variant<RGBImageBuffer, FloatImageBuffer, RGBBuffersType>>;

        class Builder
        {
        public:
            static constexpr auto DEFAULT_BORDER_COLOR = Vector4(0, 0, 0, 1);
            using Self = Builder;
            Builder();
            std::shared_ptr<Texture> build();

            /**
             * @brief sets the buffer of the texture
             *
             * @param buffer
             * @return Self&
             */
            Self &buffer(const BufferVariantType &buffer);

            /**
             * @brief sets the color of the border
             *
             * @param color
             * @return Self&
             */
            Self &border(const Vector4 &color);

            /**
             * @brief sets s-axis wrap mode
             *
             * @param mode
             * @return Self&
             */
            Self &s_wrap(GLenum mode);

            /**
             * @brief sets t-axis wrap mode
             *
             * @param mode
             * @return Self&
             */
            Self &t_wrap(GLenum mode);

            /**
             * @brief sets wrap mode in both axes
             *
             * @param mode
             * @return Self&
             */
            Self &wrap(GLenum mode);

            /**
             * @brief sets mipmap filtering mode
             *
             * @param mode
             * @return Self&
             */
            Self &min_filter(GLenum mode);

            /**
             * @brief sets texture sampling filter mode
             *
             * @param mode
             * @return Self&
             */
            Self &filter(GLenum mode);

            /**
             * @brief Internal format of the image, affects the result of
             * sampling.
             *
             * @param format
             * @return Self&
             */
            Self &format(GLenum format);

            /**
             * @brief Source image format
             *
             * @param format
             * @return Self&
             */
            Self &src_format(GLenum format);

            /**
             * @brief Sets the texture target
             *
             * @param target
             * @return Self&
             */
            Self &target(GLenum target);

        private:
            void assert_integrity();

            BufferVariantType _texture_buffer;
            std::optional<Vector4> _border_color;
            GLenum _s_wrap_mode; // "x" axis
            GLenum _t_wrap_mode; // "y" axis
            GLenum _min_filter_mode;
            GLenum _filter_mode;
            GLenum _format;
            GLenum _src_format;
            GLenum _target;
        };

        static Builder builder();

        Texture();
        Texture(GLuint texture_id, GLenum target, GLenum format);
        ~Texture();

        void set_image(const RGBImageBuffer &buffer, GLenum src_format,
                   bool generate_mipmap = true);
        void set_image(const FloatImageBuffer &buffer, GLenum src_format,
                   bool generate_mipmap = true);

        void use();

    private:
        GLuint _texture_id;
        GLenum _target;
        GLenum _format;
    };

} // namespace pogl
