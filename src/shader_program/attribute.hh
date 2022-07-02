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

    private:
        LocType _location;
        SizeType _size;
        TypeEnum _type;
        std::string _name;
        ShaderProgram *_program;
    };

} // namespace pogl
