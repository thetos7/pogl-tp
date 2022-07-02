#pragma once

#include <GL/glew.h>

#include "fwd.hh"
#include "matrix4/matrix4.hh"
#include "vector4/vector4.hh"

namespace pogl
{
    class Uniform
    {
    public:
        using LocType = GLuint;
        using SizeType = GLint;
        using TypeEnum = GLenum;
        Uniform();
        Uniform(const std::string &name, LocType location, TypeEnum type,
                SizeType size, ShaderProgram *program);

        void set_mat4(const Matrix4 &mat);
        void set_float(GLfloat value);
        void set_vec4(const Vector4 &vect);
        void set_vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        inline TypeEnum type() const
        {
            return _type;
        };

    private:
        LocType _location;
        SizeType _size;
        TypeEnum _type;
        std::string _name;
        ShaderProgram *_program;
    };
} // namespace pogl
