#pragma once

#include "texture/texture.hh"
#include "vector3/vector3.hh"

namespace pogl {
    class Particle
    {
        public:
            Particle(Vector3 position, Vector3 velocity, float life_e, float time_alive, float rotation, float scale, float tex_id);

            Vector3 getPosition() const;
            Vector3 getVelocity() const;
            float getRotation() const;
            float getScale() const;
            float getLifeExpectancy() const;
            float getTexId() const;
            void setDistanceFromCamera(Vector3 point);
            float getDistanceFromCamera() const;
            bool Update(double delta);
            void reset(Vector3 position, Vector3 velocity, float life_e, float rotation, float scale, float tex_id);
            bool isAlive();

        private:
            Vector3 position;
            Vector3 velocity;
            float tex_id; // shader needs a float
            float life_expectancy;
            float scale;
            float rotation;
            float distanceFromCamera = 0;
            float timeAlive = 0;
    };
}