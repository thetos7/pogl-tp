#include "keystate.hh"

namespace pogl
{
    Keystate::Keystate()
        : forward(false)
        , backward(false)
        , left(false)
        , right(false)
        , up(false)
        , down(false)
    {}

    Keystate &get_key_state()
    {
        static auto key_state = Keystate();
        return key_state;
    }
} // namespace pogl
