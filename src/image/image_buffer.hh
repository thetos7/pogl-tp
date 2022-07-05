#pragma once

#include <GL/glew.h>
#include <filesystem>
#include <optional>
#include <stddef.h>
#include <vector>

namespace pogl
{
    namespace fs = std::filesystem;

    template <typename PixelValueType = unsigned char>
    class ImageBuffer
    {
    public:
        using Self = ImageBuffer;
        using BufferElemType = PixelValueType;
        using BufferType = std::vector<BufferElemType>;
        using PixelType = BufferElemType *;
        using ConstPixelType = const BufferElemType *;
        using DimensionType = int;

        ImageBuffer(BufferType bytes, DimensionType width, DimensionType height,
                    DimensionType channels);
        ImageBuffer();

        static ImageBuffer sized(DimensionType width, DimensionType height,
                                 DimensionType channels);

        static std::optional<ImageBuffer> load(const fs::path &path,
                                               int channel_hint = 0);

        const BufferType &pixels() const;
        BufferType &pixels();

        const BufferElemType *data() const;
        BufferElemType *data();

        PixelType at(DimensionType row, DimensionType col);
        ConstPixelType at(DimensionType row, DimensionType col) const;

        BufferElemType &at(DimensionType row, DimensionType col,
                           DimensionType ch);
        const BufferElemType &at(DimensionType row, DimensionType col,
                                 DimensionType ch) const;

        const DimensionType &width() const;
        const DimensionType &height() const;
        const DimensionType &channels() const;

    private:
        Self &width(DimensionType value);
        Self &height(DimensionType value);
        Self &channels(DimensionType value);
        void update_dims();

        BufferType _pixel_data;
        DimensionType _width;
        DimensionType _height;
        DimensionType _channels;
        DimensionType _line_stride;
        DimensionType _size;
    };

    using RGBImageBuffer = ImageBuffer<unsigned char>;
    using FloatImageBuffer = ImageBuffer<GLfloat>;

} // namespace pogl

#include "image_buffer.hxx"
