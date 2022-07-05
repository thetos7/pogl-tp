#pragma once

#include <GL/glew.h>
#include "utils/gl_check.hh"
#include <vector>

namespace pogl {
    class RawModel {
        public:
            RawModel() = default;

            RawModel(RawModel& RM) = default;

            RawModel(GLuint VAO, int vertexCount, std::vector<GLuint> VBO_ids);

            int getVertexCount();

            GLuint getVAO();

            std::vector<GLuint> getVBOs();

        private:
            GLuint VAO;
            int vertexCount;
            std::vector<GLuint> VBO_ids;
    };
}