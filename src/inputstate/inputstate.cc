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
            << ", down: " << ks.down << " }" << std::noboolalpha;
        return out;
    }
} // namespace pogl
