#include "inputstate.hh"

namespace pogl
{
    InputState::InputState()
        : forward(false)
        , backward(false)
        , left(false)
        , right(false)
        , up(false)
        , down(false)
        , capture_cursor(true)
        , mouse_x_axis(0.0)
        , mouse_y_axis(0.0)
        , focused(true)
    {}

    InputState &get_input_state()
    {
        static auto input_state = InputState();
        return input_state;
    }

    std::ostream &operator<<(std::ostream &out, const InputState &ks)
    {
        out << std::boolalpha << "InputState { forward: " << ks.forward
            << ", backward: " << ks.backward << ", left: " << ks.left
            << ", right: " << ks.right << ", up: " << ks.up
            << ", down: " << ks.down
            << ", capture_cursor: " << ks.capture_cursor
            << ", mouse_x_axis: " << ks.mouse_x_axis
            << ", mouse_y_axis: " << ks.mouse_y_axis
            << ", focused: " << ks.focused << " }" << std::noboolalpha;
        return out;
    }
} // namespace pogl
