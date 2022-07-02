#include "attribute.hh"

namespace pogl
{
    Attribute::Attribute()
        : Attribute("", 0, 0, 0, nullptr)
    {}

    Attribute::Attribute(const std::string &name, LocType location,
                         TypeEnum type, SizeType size, ShaderProgram *program)
        : _location(location)
        , _size(size)
        , _type(type)
        , _name(name)
        , _program(program)
    {}
} // namespace pogl
