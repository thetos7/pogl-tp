#include "particle_system.hh"

#include "utils/random.hh"

namespace pogl {
    constexpr Vector3 center = Vector3(0,0,10);

    ParticleSystem::ParticleSystem(std::shared_ptr<ShaderProgram> shader)
    {
        std::vector<Particle> particles;
        this->shader = shader;
        this->particles = particles;
        generate_particles(Vector3(0,0,6), 100, 1, 2);
        ParticleRenderer PR(shader, &this->particles);
        this->renderer = PR;
    }

    bool ParticleSystem::shouldParticleReset(const Particle &particle)
    {
        return particle.getPosition().z < -1.1; // hardcode for now, parametrize later
    }

    void ParticleSystem::update(double delta) {
        for (auto &particle : particles) {
            if(shouldParticleReset(particle))
                particleReset(particle, center);
            else
                particle.Update(delta);
        }
    }
            
    void ParticleSystem::renderAllParticles(Camera camera) {
        renderer.render(particles, camera);
    }

    
    void ParticleSystem::clean() {
        renderer.clean();
    }
    
    void ParticleSystem::addParticle(Particle particle) {
        particles.push_back(particle);
    }
        
    void ParticleSystem::generate_particles(Vector3 center, float number, float speed, float life) {
        for(float i = 0; i < number; i++) {
            const auto px = center.x + float_rand_range(-3, 3);
            const auto py = center.y + float_rand_range(-3, 3);
            const auto position = Vector3(px, py, center.z);
            Vector3 velocity(float_rand_range(-0.5, 0.5), float_rand_range(-0.5, 0.5), float_rand_range(-1, -2));


            // velocity->normalized();
            // *velocity *= speed;
            const auto timeAlive = float_rand_range(0, life);
            Particle newParticle = Particle(position, velocity, life, timeAlive, 0, 1);
            addParticle(newParticle);
        }
    }

    void ParticleSystem::particleReset(Particle& particle, Vector3 center) {
        const auto px = center.x + float_rand_range(-3,3);
        const auto py = center.x + float_rand_range(-3,3);
        const auto position = Vector3(px, py, center.z);
        Vector3 velocity(float_rand_range(-0.5, 0.5), float_rand_range(-0.5, 0.5), float_rand_range(-1, -2));
        particle.reset(position, velocity, particle.getLifeExpectancy(), particle.getRotation(), particle.getScale());
    }

    void ParticleSystem::draw() {
        renderer.draw();
    }
}