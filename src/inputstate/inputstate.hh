#pragma once

#include <ostream>

namespace pogl
{
    struct InputState
    {
        InputState();
        bool forward;
        bool backward;
        bool left;
        bool right;
        bool up;
        bool down;
        bool capture_cursor;
        double mouse_x_axis;
        double mouse_y_axis;
    };

    InputState &get_input_state();
    std::ostream &operator<<(std::ostream &out, const InputState &ks);
} // namespace pogl
