#include "particle_system.hh"

#include "utils/random.hh"

namespace pogl {
    constexpr Vector3 center = Vector3(0,0,10);
    constexpr float MAX_ANGULAR_VELOCITY = 120;

    ParticleSystem::ParticleSystem(std::shared_ptr<ShaderProgram> shader, size_t textureCount)
    {
        std::vector<Particle> particles;
        this->shader = shader;
        this->respawnHeight = -1.1; // hardcode for now, parametrize later
        this->particles = particles;
        this->textureCount = (float)textureCount;
        generate_particles(Vector3(0,0,6), 300);
        ParticleRenderer PR(shader, &this->particles);
        this->renderer = PR;
    }

    bool ParticleSystem::shouldParticleReset(const Particle &particle)
    {
        return particle.getPosition().z < respawnHeight;
    }

    void ParticleSystem::update(double delta) {
        for (auto &particle : particles) {
            if(shouldParticleReset(particle))
                particleReset(particle, center);
            else
                particle.Update(delta);
        }
    }

    void ParticleSystem::clean() {
        renderer.clean();
    }
    
    void ParticleSystem::addParticle(Particle particle) {
        particles.push_back(particle);
    }
        
    void ParticleSystem::generate_particles(Vector3 center, float number) {
        for(float i = 0; i < number; i++) {
            const auto px = center.x + float_rand_range(-3, 3);
            const auto py = center.y + float_rand_range(-3, 3);
            const auto pz = float_rand_range(respawnHeight, center.z);
            const auto position = Vector3(px, py, pz);
            Vector3 velocity(float_rand_range(-0.5, 0.5), float_rand_range(-0.5, 0.5), float_rand_range(-1, -2));

            const auto texId = float_rand_range(0, textureCount);
            const auto angle = float_rand_range(0,360);
            const auto angular_velocity = float_rand_range(-MAX_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
            Particle newParticle = Particle(position, velocity, angle, angular_velocity, 1, texId);
            addParticle(newParticle);
        }
    }

    void ParticleSystem::particleReset(Particle& particle, Vector3 center) {
        const auto px = center.x + float_rand_range(-3,3);
        const auto py = center.x + float_rand_range(-3,3);
        const auto position = Vector3(px, py, center.z);
        Vector3 velocity(float_rand_range(-0.5, 0.5), float_rand_range(-0.5, 0.5), float_rand_range(-1, -2));
        const auto texId = float_rand_range(0, textureCount);
        const auto angle = float_rand_range(0,360);
        const auto angular_velocity = float_rand_range(-MAX_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
        particle.reset(position, velocity, angle, angular_velocity, particle.getScale(), texId);
    }

    void ParticleSystem::draw() {
        renderer.draw();
    }
}