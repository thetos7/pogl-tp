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
    };

    InputState &get_input_state();
    std::ostream &operator<<(std::ostream &out, const InputState &ks);
} // namespace pogl
