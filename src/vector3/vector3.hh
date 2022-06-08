#pragma once

#include <iosfwd>
#include <vector>
#include <GL/gl.h>

namespace vectors
{
    class Vector3
    {
    public:
        using ComponentType = GLfloat;

        ComponentType x;
        ComponentType y;
        ComponentType z;

        static constexpr Vector3 unit()
        {
            return Vector3{ 1, 1, 1 };
        }

        static constexpr Vector3 zero()
        {
            return Vector3{ 0, 0, 0 };
        }

        static constexpr Vector3 up()
        {
            return Vector3{ 0, 0, 1 };
        }

        static constexpr Vector3 all(ComponentType value)
        {
            return Vector3{ value, value, value };
        }

        static Vector3 from_vector(const std::vector<ComponentType> &);

        constexpr Vector3()
            : Vector3{ 0, 0, 0 }
        {}

        constexpr Vector3(ComponentType x, ComponentType y, ComponentType z)
            : x{ x }
            , y{ y }
            , z{ z }
        {}

        ComponentType dot(const Vector3 &other) const;
        Vector3 cross(const Vector3 &other) const;
        Vector3 reflect(const Vector3 &normal) const;
        ComponentType norm() const;
        ComponentType square_norm() const;
        Vector3 normalized() const;
        Vector3 lerp_to(const Vector3 &other, ComponentType factor) const;

        Vector3 &operator+=(const Vector3 &other);
        Vector3 &operator-=(const Vector3 &other);
        Vector3 &operator*=(const ComponentType &scalar);
        Vector3 &operator/=(const ComponentType &scalar);
        Vector3 &operator*=(const Vector3 &other);
        Vector3 &operator/=(const Vector3 &other);

        Vector3 operator+(const Vector3 &other) const;
        Vector3 operator-(const Vector3 &other) const;
        Vector3 operator*(const ComponentType &scalar) const;
        Vector3 operator/(const ComponentType &scalar) const;
        Vector3 operator*(const Vector3 &other) const;
        Vector3 operator/(const Vector3 &other) const;
    };

    Vector3 operator*(const Vector3::ComponentType &scalar, const Vector3 vect);
    Vector3 operator-(const Vector3 &vect);

    std::ostream &operator<<(std::ostream &out, const Vector3 &vect);
} // namespace vectors
