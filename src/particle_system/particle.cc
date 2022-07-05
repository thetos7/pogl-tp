#include "particle.hh"
#include "engine/engine.hh"
#include "vector3/vector3.hh"

namespace pogl {
    Particle::Particle(Vector3 position, Vector3 velocity, float rotation, float angular_velocity, float scale, float tex_id) {
        this->position = position;
        this->velocity= velocity;
        this->rotation = rotation;
        this->angular_velocity = angular_velocity;
        this->scale = scale;
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

    void Particle::reset(Vector3 position, Vector3 velocity, float rotation, float angular_velocity, float scale, float tex_id) {
        this->position = position;
        this->velocity= velocity;
        this->rotation = rotation;
        this->angular_velocity = angular_velocity;
        this->scale = scale;
        this->distanceFromCamera = 0;
        this->tex_id = tex_id;
    }

    void Particle::Update(double delta) {
        position += velocity * delta;
        rotation += angular_velocity * delta;
    }
}