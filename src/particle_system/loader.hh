#pragma once

#include <GL/glew.h>
#include "RawModel.hh"
#include "utils/gl_check.hh"
#include <vector>
#include "shader_program/shader_program.hh"

namespace pogl {
    class Loader {
        public:
            Loader() = default;

            RawModel LoadVAO(std::shared_ptr<ShaderProgram> shader, std::vector<float> positions, size_t particle_num);

            GLuint createVAO();

            void createUV(std::vector<GLuint> &VBO_ids);

            void Attribute(const GLchar* s, int elt_num);

            void storeData(int VBO_id, std::vector<float> positions);

            void createVBO(std::vector<GLuint> &VBO_ids);

            void unbindVBO();
            void unbindVAO();
        
        private:
            std::shared_ptr<ShaderProgram> shader;
    };
}