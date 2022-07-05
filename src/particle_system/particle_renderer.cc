#include "particle_renderer.hh"
#include "engine/engine.hh"
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
        this->vertexTexIdData = std::vector<GLfloat>(4*particles->size());
        instanceIndices = std::vector<GLint>(particles->size());
        instanceDataCounts = std::vector<GLsizei>(particles->size());
        for(size_t i = 0; i < instanceIndices.size(); i++) {
            instanceIndices[i] = i*4;
            instanceDataCounts[i] = 4;
        }
    }

    void ParticleRenderer::clean() {
        glDeleteBuffers(quad.getVBOs().size(), quad.getVBOs().data());
        GLuint VAO = quad.getVAO();
        glDeleteVertexArrays(1, &VAO);              // destroy the particles from the shader
    }

    void ParticleRenderer::genMesh() {
        const auto y_axis = -Engine::instance().main_camera->get_forward();
        const auto x_axis = Vector3::up().cross(y_axis).normalized();
        const auto z_axis = y_axis.cross(x_axis).normalized();
        const auto rotation_transform = Matrix4::basis_change(x_axis, y_axis, z_axis)
            * Matrix4::scale(0.225);

        for(size_t i = 0; i < particles->size(); i++) {
            const auto &p = particles->at(i);
            auto center = p.getPosition();
            for(size_t j = 0; j < 4; j++) {
                const auto vert_pos = (rotation_transform * Matrix4::rotate(p.getRotation(), "y") * Vector4(VERTICES[j * 2], 0, VERTICES[j * 2 + 1], 1)).to_spatial() + center;
                vertexPositionData[i*12+j*3] = vert_pos.x;
                vertexPositionData[i*12+j*3+1] = vert_pos.y;
                vertexPositionData[i*12+j*3+2] = vert_pos.z;
                vertexTexIdData[i*4+j] = p.getTexId();
            }
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, quad.getVBOs()[0]);
        CHECK_GL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertexPositionData.size() * sizeof(GLfloat), vertexPositionData.data(), GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, quad.getVBOs()[2]);
        CHECK_GL_ERROR();
        glBufferData(GL_ARRAY_BUFFER, vertexTexIdData.size() * sizeof(GLfloat), vertexTexIdData.data(), GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR();
    }


    bool CompareDistance(Particle A, Particle B);

    void ParticleRenderer::sort_particles() {
        const Vector3 cameraPositon = -Engine::instance().main_camera->get_position();
        for (auto &particle : *particles) {
           particle.setDistanceFromCamera(cameraPositon); 
        }

        std::sort(particles->begin(), particles->end(), CompareDistance);
    }

    bool CompareDistance(Particle A, Particle B) {
        return A.getDistanceFromCamera() < B.getDistanceFromCamera();
    }

    void ParticleRenderer::draw() {
        shader->use();
        glBindVertexArray(quad.getVAO());
        CHECK_GL_ERROR();
        sort_particles();
        genMesh();
        glMultiDrawArrays(GL_TRIANGLE_STRIP, instanceIndices.data(), instanceDataCounts.data(), instanceIndices.size());
        CHECK_GL_ERROR();
        glBindVertexArray(0);
        CHECK_GL_ERROR();
    }
}