#pragma once

#include <GL/glew.h>
#include <string>

#include "fwd.hh"

namespace pogl
{
    class Attribute
    {
    public:
        using LocType = GLuint;
        using SizeType = GLint;
        using TypeEnum = GLenum;
        Attribute();
        Attribute(const std::string &name, LocType location, TypeEnum type,
                  SizeType size, ShaderProgram *program);

        inline LocType location() const
        {
            return _location;
        }

        inline SizeType size() const
        {
            return _size;
        }

        inline TypeEnum type() const
        {
            return _type;
        }

    private:
        LocType _location;
        SizeType _size;
        TypeEnum _type;
        std::string _name;
        __attribute__((unused))
        ShaderProgram *_program; // current implementation does not use yet,
                                 // remove attribute once used
    };

} // namespace pogl
