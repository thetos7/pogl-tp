#include "particle.hh"
#include "engine/engine.hh"
#include "vector3/vector3.hh"

namespace pogl {
    Particle::Particle(Vector3 position, Vector3 velocity, float life_e, float time_alive, float rotation, float scale) {
        this->position = position;
        this->velocity= velocity;
        this->timeAlive = time_alive;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
        this->timeAlive = 0;
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

    Vector3 Particle::getVelocity() {
        return velocity;
    }

    float Particle::getDistanceFromCamera() {
        return distanceFromCamera;
    }

    bool Particle::isAlive() {
        return timeAlive > life_expectancy;
    }

    void Particle::reset(Vector3 position, Vector3 velocity, float life_e, float rotation, float scale) {
        this->position = position;
        this->velocity= velocity;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
        this->timeAlive = 0;
        this->distanceFromCamera = 0;
    }

    bool Particle::Update(double delta) {
        position += velocity * delta;
        timeAlive += delta;
        return timeAlive < life_expectancy;
    }
}