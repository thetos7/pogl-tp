#include "particle_renderer.hh"
#include "loader.hh"
#include "RawModel.hh"

namespace pogl {
    const std::vector<float> ParticleRenderer::VERTICES({-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f});

    ParticleRenderer::ParticleRenderer(std::shared_ptr<ShaderProgram> shader, std::vector<Particle> *particles) {
        Loader loader;
        quad = loader.LoadVAO(shader, VERTICES, particles->size());
        this->shader = shader;
        this->particles = particles;
        this->vertexPositionData = std::vector<GLfloat> (12*particles->size());
        instanceIndices = std::vector<GLint>(particles->size());
        instanceDataCounts = std::vector<GLsizei>(particles->size());
        for(int i = 0; i < instanceIndices.size(); i++) {
            instanceIndices[i] = i*4;
            instanceDataCounts[i] = 4;
        }
    }

    void ParticleRenderer::render(std::vector<Particle> particles, Camera camera) {
        Matrix4 viewMatrix = camera.get_projection();
        glBindVertexArray(quad.getVAO());
        glEnable(GL_BLEND);
        glDepthMask(false);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (auto &particle : particles) {
            glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.getVertexCount());
        }

        glDepthMask(true);
        glDisable(GL_BLEND);
        glBindVertexArray(0);
    }

    void ParticleRenderer::clean() {
        glDeleteBuffers(quad.getVBOs().size(), quad.getVBOs().data());
        GLuint VAO = quad.getVAO();
        glDeleteVertexArrays(1, &VAO);              // destroy the particles from the shader
    }

    void ParticleRenderer::genMesh() {
        for(size_t i = 0; i < particles->size(); i++) {
            auto center = particles->at(i).getPosition();
            for(size_t j = 0; j < 4; j++) {
                vertexPositionData[i*12+j*3] = VERTICES[j*2]*1.0 +center.x;
                vertexPositionData[i*12+j*3+1] = center.y;
                vertexPositionData[i*12+j*3+2] = VERTICES[j*2+1]*1.0 + center.z;
            }
        } 
        glBindBuffer(GL_ARRAY_BUFFER, quad.getVBOs()[0]);
        CHECK_GL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertexPositionData.size() * sizeof(GLfloat), vertexPositionData.data(), GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR();
    }

    void ParticleRenderer::draw() {
        shader->use();
        glBindVertexArray(quad.getVAO());
        CHECK_GL_ERROR();
        genMesh();
        glMultiDrawArrays(GL_TRIANGLE_STRIP, instanceIndices.data(), instanceDataCounts.data(), instanceIndices.size());
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }
}