#include "image_buffer.hh"

#include <iostream>
#include <stdexcept>

#include "stb_image.h"

namespace pogl
{
    ImageBuffer::ImageBuffer(BufferType bytes, DimensionType width,
                             DimensionType height, DimensionType channels)
        : _bytes(bytes)
        , _width(width)
        , _height(height)
        , _channels(channels)
        , _line_stride(width * channels)
        , _size(width * height * channels)
    {}

    std::optional<ImageBuffer> ImageBuffer::load(const fs::path &path,
                                                 int channel_hint)
    {
        int width = 0;
        int height = 0;
        int channel_count = 0;
        auto bytes = stbi_load(path.c_str(), &width, &height, &channel_count,
                               channel_hint);
        if (bytes == nullptr)
        {
            return std::nullopt;
        }

        return ImageBuffer(bytes, width, height, channel_count);
    }

    ImageBuffer::~ImageBuffer()
    {
        stbi_image_free(_bytes);
    }

    ImageBuffer::ConstBufferType ImageBuffer::data() const
    {
        return _bytes;
    }
    ImageBuffer::BufferType ImageBuffer::data()
    {
        return _bytes;
    }

    ImageBuffer::BufferType ImageBuffer::at(DimensionType row,
                                            DimensionType col)
    {
        DimensionType idx = row * _line_stride + col * _channels;
        if (idx >= _size)
        {
            std::cerr << "ERROR: Out of bounds access, tried to access pixel "
                         "{col:"
                      << col << ", row: " << row
                      << " }, dimensions are {width: " << _width
                      << ", height: " << _height << ", channels: " << _channels
                      << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        return &_bytes[idx];
    }

    ImageBuffer::ConstBufferType ImageBuffer::at(DimensionType row,
                                                 DimensionType col) const
    {
        DimensionType idx = row * _line_stride + col * _channels;
        if (idx >= _size)
        {
            std::cerr
                << "ERROR: Out of bounds access, tried to access pixel {col:"
                << col << ", row: " << row
                << " }, dimensions are {width: " << _width
                << ", height: " << _height << ", channels: " << _channels
                << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        return &_bytes[idx];
    }

    ImageBuffer::BufferElemType &
    ImageBuffer::at(DimensionType row, DimensionType col, DimensionType ch)
    {
        DimensionType idx = row * _line_stride + col * _channels + ch;
        if (idx >= _size)
        {
            std::cerr
                << "ERROR: Out of bounds access, tried to access pixel {col:"
                << col << ", row: " << row << ", ch: " << ch
                << " }, dimensions are {width: " << _width
                << ", height: " << _height << ", channels: " << _channels
                << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        return _bytes[idx];
    }

    const ImageBuffer::BufferElemType &ImageBuffer::at(DimensionType row,
                                                       DimensionType col,
                                                       DimensionType ch) const
    {
        DimensionType idx = row * _line_stride + col * _channels + ch;
        if (idx >= _size)
        {
            std::cerr
                << "ERROR: Out of bounds access, tried to access pixel {col:"
                << col << ", row: " << row << ", ch: " << ch
                << " }, dimensions are {width: " << _width
                << ", height: " << _height << ", channels: " << _channels
                << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        return _bytes[idx];
    }

    const ImageBuffer::DimensionType &ImageBuffer::width() const
    {
        return _width;
    }
    const ImageBuffer::DimensionType &ImageBuffer::height() const
    {
        return _height;
    }
    const ImageBuffer::DimensionType &ImageBuffer::channels() const
    {
        return _channels;
    }
} // namespace pogl
