#pragma once

#include <GL/glew.h>
#include <vector>
#include "particle.hh"
#include "shader_program/shader_program.hh"
#include "matrix4/matrix4.hh"
#include "camera/camera.hh"
#include "RawModel.hh"

namespace pogl {
    class ParticleRenderer
    {
        public:
            static const std::vector<float> VERTICES;

            ParticleRenderer() = default;

            ParticleRenderer(ParticleRenderer& PR) = default;

            ParticleRenderer(std::shared_ptr<ShaderProgram> shader, std::vector<Particle> *particles);

            ~ParticleRenderer() = default;

            void render(std::vector<Particle> particles, Camera camera);

            /**
             * @brief Re-orient the particles to the view of the camera.
             */
            void updateModelViewMatrix(Vector3 position, float rotation, float scale, Matrix4 ViewMatrix);

            void genMesh();
            void draw();

            void clean();

        private:
            RawModel quad;
            std::vector<Particle> *particles;
            std::shared_ptr<ShaderProgram> shader;
            std::vector<GLfloat> vertexPositionData;
            std::vector<GLint> instanceIndices;
            std::vector<GLsizei> instanceDataCounts;
    };
}