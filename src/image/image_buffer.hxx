#pragma once

#include <iostream>
#include <stdexcept>

#include "image_buffer.hh"
#include "stb_image.h"
#include "utils/log.hh"

namespace pogl
{

    template <typename T>
    ImageBuffer<T>::ImageBuffer()
        : ImageBuffer(BufferType(), 0, 0, 0)
    {}

    template <typename T>
    ImageBuffer<T>::ImageBuffer(BufferType bytes, DimensionType width,
                                DimensionType height, DimensionType channels)
        : _pixel_data(bytes)
        , _width(width)
        , _height(height)
        , _channels(channels)
        , _line_stride(width * channels)
        , _size(width * height * channels)
    {}

    template <typename T>
    ImageBuffer<T> ImageBuffer<T>::sized(DimensionType width,
                                         DimensionType height,
                                         DimensionType channels)
    {
        auto bytes = BufferType(width * height * channels);
        return ImageBuffer(bytes, width, height, channels);
    }

    template <typename T>
    const typename ImageBuffer<T>::BufferElemType *ImageBuffer<T>::data() const
    {
        return _pixel_data.data();
    }
    template <typename T>
    typename ImageBuffer<T>::BufferElemType *ImageBuffer<T>::data()
    {
        return _pixel_data.data();
    }

    template <typename T>
    const typename ImageBuffer<T>::BufferType &ImageBuffer<T>::pixels() const
    {
        return _pixel_data;
    }

    template <typename T>
    typename ImageBuffer<T>::BufferType &ImageBuffer<T>::pixels()
    {
        return _pixel_data;
    }

    template <typename T>
    typename ImageBuffer<T>::PixelType ImageBuffer<T>::at(DimensionType row,
                                                          DimensionType col)
    {
        if (row >= _height || col >= _width)
        {
            std::cerr << LOG_ERROR
                      << "Out of bounds access, tried to access pixel "
                         "{col:"
                      << col << ", row: " << row
                      << " }, dimensions are {width: " << _width
                      << ", height: " << _height << ", channels: " << _channels
                      << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        DimensionType idx = row * _line_stride + col * _channels;
        return &_pixel_data[idx];
    }

    template <typename T>
    typename ImageBuffer<T>::ConstPixelType
    ImageBuffer<T>::at(DimensionType row, DimensionType col) const
    {
        if (row >= _height || col >= _width)
        {
            std::cerr << LOG_ERROR
                      << "Out of bounds access, tried to access pixel {col:"
                      << col << ", row: " << row
                      << " }, dimensions are {width: " << _width
                      << ", height: " << _height << ", channels: " << _channels
                      << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        DimensionType idx = row * _line_stride + col * _channels;
        return &_pixel_data[idx];
    }

    template <typename T>
    typename ImageBuffer<T>::BufferElemType &
    ImageBuffer<T>::at(DimensionType row, DimensionType col, DimensionType ch)
    {
        if (row >= _height || col >= _width || ch >= _channels)
        {
            std::cerr << LOG_ERROR
                      << "Out of bounds access, tried to access pixel {col:"
                      << col << ", row: " << row << ", ch: " << ch
                      << " }, dimensions are {width: " << _width
                      << ", height: " << _height << ", channels: " << _channels
                      << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        DimensionType idx = row * _line_stride + col * _channels + ch;
        return _pixel_data[idx];
    }

    template <typename T>
    const typename ImageBuffer<T>::BufferElemType &
    ImageBuffer<T>::at(DimensionType row, DimensionType col,
                       DimensionType ch) const
    {
        if (row >= _height || col >= _width || ch >= _channels)
        {
            std::cerr << LOG_ERROR
                      << "Out of bounds access, tried to access pixel {col:"
                      << col << ", row: " << row << ", ch: " << ch
                      << " }, dimensions are {width: " << _width
                      << ", height: " << _height << ", channels: " << _channels
                      << " }\n";
            throw std::logic_error("Out of bounds acces to image buffer");
        }
        DimensionType idx = row * _line_stride + col * _channels + ch;
        return _pixel_data[idx];
    }

    template <typename T>
    const typename ImageBuffer<T>::DimensionType &ImageBuffer<T>::width() const
    {
        return _width;
    }
    template <typename T>
    const typename ImageBuffer<T>::DimensionType &ImageBuffer<T>::height() const
    {
        return _height;
    }
    template <typename T>
    const typename ImageBuffer<T>::DimensionType &
    ImageBuffer<T>::channels() const
    {
        return _channels;
    }

    template <typename T>
    ImageBuffer<T> &ImageBuffer<T>::width(DimensionType value)
    {
        _width = value;
        update_dims();
        return *this;
    }
    template <typename T>
    ImageBuffer<T> &ImageBuffer<T>::height(DimensionType value)
    {
        _height = value;
        update_dims();
        return *this;
    }
    template <typename T>
    ImageBuffer<T> &ImageBuffer<T>::channels(DimensionType value)
    {
        _channels = value;
        update_dims();
        return *this;
    }
    template <typename T>
    void ImageBuffer<T>::update_dims()
    {
        _line_stride = _width * _channels;
        _size = _width * _height * _channels;
    }
} // namespace pogl
