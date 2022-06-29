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

    std::ostream &operator<<(std::ostream &out, const Keystate &ks)
    {
        out << std::boolalpha << "Keystate { forward: " << ks.forward
            << ", backward: " << ks.backward << ", left: " << ks.left
            << ", right: " << ks.right << ", up: " << ks.up
            << ", down: " << ks.down << " }" << std::noboolalpha;
        return out;
    }
} // namespace pogl
