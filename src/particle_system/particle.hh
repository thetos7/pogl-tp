#pragma once

#include "vector3/vector3.hh"

namespace pogl {
    class Particle
    {
        public:
            static constexpr float GRAVITY = -3;

            Particle(Vector3 position, Vector3 velocity, float gravity, float life_e, float rotation, float scale);

            Vector3 getPosition();
            Vector3 getVelocity();
            float getRotation();
            float getScale();
            float getLifeExpectancy();
            float getGravity();
            bool Update(double delta);
            void reset(Vector3 position, Vector3 velocity, float gravity, float life_e, float rotation, float scale);
            bool isAlive();

        private:
            Vector3 position;
            Vector3 velocity;
            float gravity;
            float life_expectancy;
            float scale;
            float rotation;
            float timeAlive = 0;
    };
}