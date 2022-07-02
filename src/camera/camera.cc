#include "camera.hh"

#include <cmath>

#include "inputstate/inputstate.hh"

namespace pogl
{
    using Self = Camera;

    // TODO simplify api by giving simpler projection settings like width,
    // aspect ratio, fov, near clip and far clip
    Camera::Camera(const Vector3 &initial_position, double pitch, double yaw,
                   const Matrix4 &projection)
        : _position(initial_position)
        , _pitch(pitch)
        , _yaw(yaw)
        , _projection(projection)
    {}

    Vector3 Camera::get_forward() const
    {
        return Vector3{
            static_cast<Vector3::ComponentType>(std::cos(_yaw)
                                                * std::cos(_pitch)),
            static_cast<Vector3::ComponentType>(std::sin(_yaw)
                                                * std::cos(_pitch)),
            static_cast<Vector3::ComponentType>(std::sin(_pitch)),
        }
            .normalized();
    }

    Matrix4 Camera::get_transform() const
    {
        const auto forward = get_forward();

        const auto right = forward.cross(Self::UP).normalized();
        const auto view_up = right.cross(forward).normalized();

        const auto eyeTranslate =
            Matrix4::translation(-_position.x, -_position.y, -_position.z);

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

    Matrix4 Camera::get_projection() const
    {
        return _projection;
    }

    Self &Camera::move_relative(const Vector3 &movement)
    {
        const auto forward = Vector3(std::cos(_yaw), std::sin(_yaw), 0);
        const auto right =
            Vector3(std::cos(_yaw - M_PI_2), std::sin(_yaw - M_PI_2), 0);
        const auto global_space_movement =
            movement.x * forward + movement.y * right + movement.z * Self::UP;

        _position += global_space_movement;
        return *this;
    }

    void Camera::set_projection(const Matrix4 &projection)
    {
        _projection = projection;
    }

    void Camera::update(double delta)
    {
        auto &input_state = get_input_state();

        if (!input_state.focused)
        {
            return;
        }

        if (input_state.capture_cursor)
        {
            // update pitch
            const auto pitch_movement =
                -input_state.mouse_y_axis * LOOK_SENSITIVITY * M_PI * 2.;
            _pitch += pitch_movement;
            if (_pitch > M_PI_2) // upper pitch limit, looking "up"
            {
                _pitch = M_PI_2;
            }
            else if (_pitch < -M_PI_2) // looking "down"
            {
                _pitch = -M_PI_2;
            }

            // update yaw
            const auto yaw_movement =
                -input_state.mouse_x_axis * LOOK_SENSITIVITY * M_PI * 2.;
            _yaw += yaw_movement;
            _yaw = std::fmod(_yaw, 2. * M_PI); // keep yaw in range [0; 2PI)
        }
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
