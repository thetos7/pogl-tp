#pragma once

#include <GL/glew.h>
#include <assimp/mesh.h>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <optional>

namespace pogl
{
    namespace fs = std::filesystem;

    class Importer
    {
    public:
        using BufferType = std::vector<GLfloat>;
        using ResultType = std::map<std::string, BufferType>;
        using Self = Importer;
        using BufferExtractor =
            std::function<void(BufferType &buffer, unsigned int vert_idx, aiMesh *mesh)>;
        using ExtractorMap = std::map<std::string, BufferExtractor>;

        static void extract_position(BufferType &buffer, unsigned int idx,
                                     aiMesh *mesh);
        static void extract_texcoords(BufferType &buffer, unsigned int idx,
                                      aiMesh *mesh);
        static void extract_normals(BufferType &buffer, unsigned int idx,
                                      aiMesh *mesh);

        Importer(const fs::path &path);

        static Importer read_file(const fs::path &path);

        Self &configure_buffer(const std::string &name,
                               BufferExtractor extractor);
        Self &set_flags(unsigned int flags);
        Self &add_flags(unsigned int flags);
        Self &remove_flags(unsigned int flags);

        std::optional<ResultType> import();

    private:
        fs::path _path;
        unsigned int _flags;
        ExtractorMap _extractors;
    };

} // namespace pogl
