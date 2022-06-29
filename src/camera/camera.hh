#pragma once

#include "matrix4/matrix4.hh"
#include "vector3/vector3.hh"

namespace pogl
{
    class Camera
    {
    public:
        using Self = Camera;

        static constexpr auto UP = Vector3(0, 0, 1);
        static constexpr double SPEED = 0.5;

        Camera(const Vector3 &initial_position, double pitch, double yaw);
        Matrix4 get_transform() const;
        Self &move_relative(const Vector3 &movement);

        void update(double delta);

    private:
        Vector3 position_;
        double pitch_;
        double yaw_;
    };

} // namespace pogl
