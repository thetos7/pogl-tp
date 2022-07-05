#pragma once

#include "texture/texture.hh"
#include "vector3/vector3.hh"

namespace pogl {
    class Particle
    {
        public:
            Particle(Vector3 position, Vector3 velocity, float rotation, float angular_velocity, float scale, float tex_id);

            Vector3 getPosition() const;
            Vector3 getVelocity() const;
            float getRotation() const;
            float getScale() const;
            float getTexId() const;
            void setDistanceFromCamera(Vector3 point);
            float getDistanceFromCamera() const;
            void Update(double delta);
            void reset(Vector3 position, Vector3 velocity, float rotation, float angular_velocity, float scale, float tex_id);

        private:
            Vector3 position;
            Vector3 velocity;
            float tex_id; // shader needs a float
            float scale;
            float angular_velocity;
            float rotation;
            float distanceFromCamera = 0;
    };
}