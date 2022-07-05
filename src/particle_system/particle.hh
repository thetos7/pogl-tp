#pragma once

#include "texture/texture.hh"
#include "vector3/vector3.hh"

namespace pogl {
    class Particle
    {
        public:
            Particle(Vector3 position, Vector3 velocity, float life_e, float rotation, float scale);
            Vector3 getPosition();
            Vector3 getVelocity();
            float getRotation();
            float getScale();
            float getLifeExpectancy();
            float getDistanceFromCamera();
            bool Update(double delta);
            void reset(Vector3 position, Vector3 velocity, float life_e, float rotation, float scale);
            bool isAlive();

        private:
            Vector3 position;
            Vector3 velocity;
            float life_expectancy;
            float scale;
            float rotation;
            float distanceFromCamera = 0;
            float timeAlive = 0;
    };
}