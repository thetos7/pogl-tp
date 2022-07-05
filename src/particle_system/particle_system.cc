#include "particle_system.hh"

namespace pogl {
    constexpr Vector3 center = Vector3(0,0,10);

    ParticleSystem::ParticleSystem(std::shared_ptr<ShaderProgram> shader)
    {
        std::vector<Particle> particles;
        this->shader = shader;
        this->particles = particles;
        generate_particles(Vector3(0,0,6), 1, 1, -1, 2);
        ParticleRenderer PR(shader, &this->particles);
        this->renderer = PR;
    }
    
    void ParticleSystem::update(double delta) {
        for (auto &particle : particles) {
            if(particle.isAlive())
                particle.Update(delta);
            else
                particleReset(particle, center);
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
        
    void ParticleSystem::generate_particles(Vector3 center, float number, float speed, float gravity, float life) {
        for(float i = 0; i < number; i++) {
            center.x += (std::rand()%15) - 14;
            center.y += (std::rand()%15) - 14;
            float x = center.x + std::rand() % 10;
            float z = center.z + std::rand() % 10;
            Vector3* velocity = new Vector3(x, 1, z);
            velocity->normalized();
            *velocity *= speed;
            Particle newParticle = Particle(center, *velocity, gravity, life, 0, 1);
            addParticle(newParticle);
        }
    }

    void ParticleSystem::particleReset(Particle& particle, Vector3 center) {
        center.x += (std::rand()%15) - 10;
        center.y += (std::rand()%15) - 10;
        particle.reset(center, particle.getVelocity(), particle.getGravity(), particle.getLifeExpectancy(), particle.getRotation(), particle.getScale());
    }

    void ParticleSystem::draw() {
        renderer.draw();
    }
}