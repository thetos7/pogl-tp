#pragma once

#include <ostream>

namespace pogl
{
    struct Keystate
    {
        Keystate();
        bool forward;
        bool backward;
        bool left;
        bool right;
        bool up;
        bool down;
    };

    Keystate &get_key_state();
    std::ostream &operator<<(std::ostream &out, const Keystate &ks);
} // namespace pogl
