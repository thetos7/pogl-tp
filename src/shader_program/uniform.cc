#include "uniform.hh"

#include <sstream>

#include "shader_program/shader_program.hh"
#include "utils/gl_check.hh"
#include "vector3/vector3.hh"
#include "vector4/vector4.hh"

namespace pogl
{
    Uniform::Uniform()
        : Uniform("", 0, 0, 0, nullptr)
    {}

    Uniform::Uniform(const std::string &name, LocType location, TypeEnum type,
                     SizeType size, ShaderProgram *program)
        : _location(location)
        , _size(size)
        , _type(type)
        , _name(name)
        , _program(program)
    {}

    void Uniform::set_mat4(const Matrix4 &mat)
    {
        if (_type != GL_FLOAT_MAT4)
        {
            std::ostringstream oss;
            oss << "Error: Attempt to set uniform `" << _name
                << "` with value of type mat4, but its type differs.\n";
            throw std::logic_error(oss.str());
        }
        _program->use();
        glUniformMatrix4fv(_location, 1, GL_TRUE, mat.data());
        CHECK_GL_ERROR();
    }

    void Uniform::set_float(GLfloat value)
    {
        if (_type != GL_FLOAT)
        {
            std::ostringstream oss;
            oss << "Error: Attempt to set uniform `" << _name
                << "` with value of type float, but its type differs.\n";
            throw std::logic_error(oss.str());
        }
        _program->use();
        glUniform1f(_location, value);
        CHECK_GL_ERROR();
    }

    void Uniform::set_vec3(const Vector3 &vect)
    {
        set_vec3(vect.x, vect.y, vect.z);
    }
    void Uniform::set_vec3(GLfloat x, GLfloat y, GLfloat z)
    {
        if (_type != GL_FLOAT_VEC3)
        {
            std::ostringstream oss;
            oss << "Error: Attempt to set uniform `" << _name
                << "` with value of type vec3, but its type differs.\n";
            throw std::logic_error(oss.str());
        }
        _program->use();
        glUniform3f(_location, x, y, z);
        CHECK_GL_ERROR();
    }

    void Uniform::set_int(GLint value)
    {
        if (_type != GL_INT && _type != GL_SAMPLER_2D)
        {
            std::ostringstream oss;
            oss << "Error: Attempt to set uniform `" << _name
                << "` with value of type int, but its type isn't int or a "
                   "sampler.\n";
            throw std::logic_error(oss.str());
        }
        _program->use();
        glUniform1i(_location, value);
        CHECK_GL_ERROR();
    }

    void Uniform::set_vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        if (_type != GL_FLOAT_VEC4)
        {
            std::ostringstream oss;
            oss << "Error: Attempt to set uniform `" << _name
                << "` with value of type vec4, but its type differs.\n";
            throw std::logic_error(oss.str());
        }
        _program->use();
        glUniform4f(_location, x, y, z, w);
        CHECK_GL_ERROR();
    }

    void Uniform::set_vec4(const Vector4 &vect)
    {
        set_vec4(vect.x, vect.y, vect.z, vect.w);
    }
} // namespace pogl
