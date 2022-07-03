#include "importer.hh"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace pogl
{
    using Self = Importer;
    void Importer::extract_position(BufferType &buffer, unsigned int vert_idx,
                                    aiMesh *mesh)
    {
        auto vert = mesh->mVertices[vert_idx];

        buffer.push_back(vert.x);
        buffer.push_back(vert.y);
        buffer.push_back(vert.z);
    }

    void Importer::extract_texcoords(BufferType &buffer, unsigned int vert_idx,
                                     aiMesh *mesh)
    {
        auto uv = mesh->mTextureCoords[0][vert_idx];

        buffer.push_back(uv.x);
        buffer.push_back(uv.y);
    }

    Importer::Importer(const fs::path &path)
        : _path(path)
        , _flags(aiProcess_CalcTangentSpace | aiProcess_Triangulate
                 | aiProcess_FlipUVs)
        , _extractors()
    {}

    Importer Importer::read_file(const fs::path &path)
    {
        return Importer(path);
    }

    Self &Importer::configure_buffer(const std::string &name,
                                     BufferExtractor extractor)
    {
        _extractors.emplace(name, extractor);
        return *this;
    }

    Self &Importer::set_flags(unsigned int flags)
    {
        _flags = flags;
        return *this;
    }

    Self &Importer::add_flags(unsigned int flags)
    {
        _flags |= flags;
        return *this;
    }

    Self &Importer::remove_flags(unsigned int flags)
    {
        _flags &= ~flags;
        return *this;
    }

    std::optional<Importer::ResultType> Importer::import()
    {
        Assimp::Importer importer;
        auto scene = importer.ReadFile(_path.c_str(), _flags);
        if (scene == nullptr)
        {
            return std::nullopt;
        }

        const auto idx = scene->mRootNode->mChildren[0]->mMeshes[0];
        const auto mesh = scene->mMeshes[idx];

        auto output = ResultType();

        for (auto [name, _] : _extractors)
        {
            output.emplace(name, BufferType());
        }

        for (size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            auto face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; ++j)
            {
                auto vert_idx = face.mIndices[j];
                for (auto [name, extract] : _extractors)
                {
                    extract(output.at(name), vert_idx, mesh);
                }
            }
        }

        return output;
    }
} // namespace pogl
