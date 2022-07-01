#pragma once

#include "matrix4/matrix4.hh"
#include "properties/updateable.hh"
#include "vector3/vector3.hh"

namespace pogl
{
    class Camera : public Updateable
    {
    public:
        using Self = Camera;

        static constexpr auto UP = Vector3(0, 0, 1);
        static constexpr double SPEED = 2.0;
        static constexpr double LOOK_SENSITIVITY = 0.01;

        Camera(const Vector3 &initial_position, double pitch, double yaw,
               const Matrix4 &projection);
        virtual ~Camera() = default;

        Matrix4 get_transform() const;
        Matrix4 get_projection() const;
        Vector3 get_forward() const;
        Self &move_relative(const Vector3 &movement);

        virtual void update(double delta) override;

    private:
        Vector3 position_;
        double pitch_;
        double yaw_;
        Matrix4 projection_;
    };

} // namespace pogl
