#include "camera.hh"

#include <cmath>

#include "inputstate/inputstate.hh"

namespace pogl
{
    using Self = Camera;

    Camera::Camera(const Vector3 &initial_position, double pitch, double yaw)
        : position_(initial_position)
        , pitch_(pitch)
        , yaw_(yaw)
    {}

    Vector3 Camera::get_forward() const
    {
        return Vector3{
            static_cast<Vector3::ComponentType>(std::cos(yaw_)
                                                * std::cos(pitch_)),
            static_cast<Vector3::ComponentType>(std::sin(yaw_)
                                                * std::cos(pitch_)),
            static_cast<Vector3::ComponentType>(std::sin(pitch_)),
        }
            .normalized();
    }

    Matrix4 Camera::get_transform() const
    {
        const auto forward = get_forward();

        const auto right = forward.cross(Self::UP).normalized();
        const auto view_up = right.cross(forward).normalized();

        const auto eyeTranslate =
            Matrix4::translation(-position_.x, -position_.y, -position_.z);

        // clang-format off
        const auto rotation = Matrix4(Matrix4::ElementsBufferType{
            right.x   , right.y   , right.z   , 0, // l1
            view_up.x , view_up.y , view_up.z , 0, // l2
            -forward.x, -forward.y, -forward.z, 0, // l3
            0         , 0         , 0         , 1 //  l4
        });
        // clang-format on

        return rotation * eyeTranslate;
    }

    Self &Camera::move_relative(const Vector3 &movement)
    {
        const auto forward = Vector3(std::cos(yaw_), std::sin(yaw_), 0);
        const auto right =
            Vector3(std::cos(yaw_ - M_PI_2), std::sin(yaw_ - M_PI_2), 0);
        const auto global_space_movement =
            movement.x * forward + movement.y * right + movement.z * Self::UP;

        position_ += global_space_movement;
        return *this;
    }

    void Camera::update(double delta)
    {
        auto &input_state = get_input_state();

        // update pitch
        const auto pitch_movement =
            -input_state.mouse_y_axis * LOOK_SENSITIVITY * M_PI * 2.;
        pitch_ += pitch_movement;
        if (pitch_ > M_PI_2) // upper pitch limit, looking "up"
        {
            pitch_ = M_PI_2;
        }
        else if (pitch_ < -M_PI_2) // looking "down"
        {
            pitch_ = -M_PI_2;
        }

        // update yaw
        const auto yaw_movement =
            -input_state.mouse_x_axis * LOOK_SENSITIVITY * M_PI * 2.;
        yaw_ += yaw_movement;
        yaw_ = std::fmod(yaw_, 2. * M_PI); // keep yaw in range [0; 2PI)

        // update position
        const auto x_input =
            (int)input_state.forward - (int)input_state.backward;
        const auto y_input = (int)input_state.right - (int)input_state.left;
        const auto z_input = (int)input_state.up - (int)input_state.down;

        const auto movement_direction =
            Vector3(x_input, y_input, z_input).normalized();
        const auto movement = movement_direction * SPEED * delta;
        move_relative(movement);
    }

} // namespace pogl
