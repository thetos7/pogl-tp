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
    }

    bool Particle::Update(double delta) {
        velocity.y += GRAVITY * gravity * delta; // we may want to add the second
        Vector3 new_velocity(velocity);
        new_velocity * delta;
        new_velocity + position;
        timeAlive += delta;
        return timeAlive < life_expectancy;
    }
}