#pragma once

#include <GL/glew.h>
#include <vector>
#include "shader_program/shader_program.hh"
#include "matrix4/matrix4.hh"
#include "camera/camera.hh"
#include "particle_renderer.hh"
#include "properties/drawable.hh"

namespace pogl {
    class ParticleSystem : public Updateable, public Drawable
    {
        public:
            ParticleSystem(std::shared_ptr<ShaderProgram> shader);

            virtual ~ParticleSystem() = default;

            void update(double delta);
            void renderAllParticles(Camera camera);
            void clean();
            void addParticle(Particle particle);

            void generate_particles(Vector3 center, float number, float speed, float life);

            void particleReset(Particle& particle, Vector3 center);

            void draw();

        private:
            std::vector<Particle> particles;
            ParticleRenderer renderer;
            std::shared_ptr<ShaderProgram> shader;
    };
}