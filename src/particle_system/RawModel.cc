#include "RawModel.hh"

namespace pogl {
    RawModel::RawModel(GLuint VAO, int vertexCount, std::vector<GLuint> VBO_ids)
        : VAO(VAO)
        , vertexCount(vertexCount)
        , VBO_ids(VBO_ids)
    {}

    int RawModel::getVertexCount() {
        return vertexCount;
    }

    GLuint RawModel::getVAO() {
        return VAO;
    }

    std::vector<GLuint> RawModel::getVBOs() {
        return VBO_ids;
    }
}