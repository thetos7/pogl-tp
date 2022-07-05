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
    }

    void ParticleRenderer::render(std::vector<Particle> particles, Camera camera) {
        Matrix4 viewMatrix = camera.get_projection();
        glBindVertexArray(quad.getVAO());
        glEnable(GL_BLEND);
        glDepthMask(false);

        for (auto &particle : particles) {
            //updateModelViewMatrix(current_particle.position, current_particle.rotation, current_particle.scale, viewMatrix);
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

    void ParticleRenderer::draw() {
        shader->use();
        glBindVertexArray(quad.getVAO());
        CHECK_GL_ERROR();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }
}