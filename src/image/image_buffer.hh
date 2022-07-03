#pragma once

#include <filesystem>
#include <stddef.h>
#include <optional>

namespace pogl
{
    namespace fs = std::filesystem;

    class ImageBuffer
    {
    public:
        using BufferElemType = unsigned char;
        using BufferType = unsigned char *;
        using ConstBufferType = const unsigned char *;
        using DimensionType = int;

        ImageBuffer(BufferType bytes, DimensionType width, DimensionType height,
                    DimensionType channels);
        ~ImageBuffer();

        static std::optional<ImageBuffer> load(const fs::path &path, int channel_hint = 0);

        ConstBufferType data() const;
        BufferType data();

        BufferType at(DimensionType row, DimensionType col);
        ConstBufferType at(DimensionType row, DimensionType col) const;

        BufferElemType &at(DimensionType row, DimensionType col,
                           DimensionType ch);
        const BufferElemType &at(DimensionType row, DimensionType col,
                                 DimensionType ch) const;

        const DimensionType &width() const;
        const DimensionType &height() const;
        const DimensionType &channels() const;

    private:
        BufferType _bytes;
        DimensionType _width;
        DimensionType _height;
        DimensionType _channels;
        DimensionType _line_stride;
        DimensionType _size;
    };

} // namespace pogl
