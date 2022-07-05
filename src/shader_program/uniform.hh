#pragma once

#include <GL/glew.h>
#include <string>

#include "fwd.hh"
#include "matrix4/matrix4.hh"
#include "vector3/fwd.hh"
#include "vector4/fwd.hh"

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
        void set_vec3(const Vector3 &vect);
        void set_vec3(GLfloat x, GLfloat y, GLfloat z);
        void set_int(GLint value);

        inline TypeEnum type() const
        {
            return _type;
        }

        inline SizeType size() const
        {
            return _size;
        }

    private:
        LocType _location;
        SizeType _size;
        TypeEnum _type;
        std::string _name;
        ShaderProgram *_program;
    };
} // namespace pogl
