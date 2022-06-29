#pragma once

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
} // namespace pogl
