#include "particle.hh"
#include "engine/engine.hh"
#include "vector3/vector3.hh"

namespace pogl {
    Particle::Particle(Vector3 position, Vector3 velocity, float life_e, float time_alive, float rotation, float scale, float tex_id) {
        this->position = position;
        this->velocity= velocity;
        this->timeAlive = time_alive;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
        this->timeAlive = 0;
        this->tex_id = tex_id;
    }

    Vector3 Particle::getPosition() const {
        return position;
    }

    float Particle::getRotation() const {
        return rotation;
    }

    float Particle::getScale() const {
        return scale;
    }

    float Particle::getLifeExpectancy() const {
        return life_expectancy;
    }

    Vector3 Particle::getVelocity() const {
        return velocity;
    }

    float Particle::getTexId() const {
        return tex_id;
    }

    void Particle::setDistanceFromCamera(Vector3 point) {

        this->distanceFromCamera = (point - position).norm();
    }

    float Particle::getDistanceFromCamera() const {
        return distanceFromCamera;
    }

    bool Particle::isAlive() {
        return timeAlive > life_expectancy;
    }

    void Particle::reset(Vector3 position, Vector3 velocity, float life_e, float rotation, float scale, float tex_id) {
        this->position = position;
        this->velocity= velocity;
        this->life_expectancy = life_e;
        this->rotation = rotation;
        this->scale = scale;
        this->timeAlive = 0;
        this->distanceFromCamera = 0;
        this->tex_id = tex_id;
    }

    bool Particle::Update(double delta) {
        position += velocity * delta;
        timeAlive += delta;
        return timeAlive < life_expectancy;
    }
}