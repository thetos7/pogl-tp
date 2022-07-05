#include "particle.hh"

namespace pogl {
    Particle::Particle(Vector3 position, Vector3 velocity, float gravity, float life_e, float rotation, float scale) {
        this->position = position;
        this->velocity= velocity;
        this->gravity = gravity;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
    }

    Vector3 Particle::getPosition() {
        return position;
    }

    float Particle::getRotation() {
        return rotation;
    }

    float Particle::getScale() {
        return scale;
    }

    float Particle::getLifeExpectancy() {
        return life_expectancy;
    }

    float Particle::getGravity() {
        return gravity;
    }

    Vector3 Particle::getVelocity() {
        return velocity;
    }

    bool Particle::isAlive() {
        return timeAlive > life_expectancy;
    }

    void Particle::reset(Vector3 position, Vector3 velocity, float gravity, float life_e, float rotation, float scale) {
        this->position = position;
        this->velocity= velocity;
        this->gravity = gravity;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
        this->timeAlive = 0;
    }

    bool Particle::Update(double delta) {
        // velocity.z += GRAVITY * delta; // we may want to add the second
        position += velocity * delta;
        timeAlive += delta;
        return timeAlive < life_expectancy;
    }
}