#include "image_buffer.hh"

namespace pogl
{
    template <>
    std::optional<RGBImageBuffer> RGBImageBuffer::load(const fs::path &path,
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
        const size_t size = width * height * channel_count;
        auto bytes_vector = BufferType(size);
        bytes_vector.assign(bytes, bytes + size);
        stbi_image_free(bytes);

        return RGBImageBuffer(bytes_vector, width, height, channel_count);
    }

    template <>
    std::optional<FloatImageBuffer> FloatImageBuffer::load(const fs::path &path,
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
        const size_t size = width * height * channel_count;
        auto bytes_vector = BufferType(size);
        bytes_vector.assign(bytes, bytes + size);
        std::transform(bytes_vector.begin(), bytes_vector.end(),
                       bytes_vector.begin(), [](auto v) { return v / 255.; });
        stbi_image_free(bytes);

        return ImageBuffer(bytes_vector, width, height, channel_count);
    }
} // namespace pogl
