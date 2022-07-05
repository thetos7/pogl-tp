#pragma once

#include <GL/glew.h>
#include <iosfwd>
#include <vector>

#include "vector4/fwd.hh"

namespace pogl
{
    class Vector3
    {
    public:
        using ComponentType = GLfloat;

        ComponentType x;
        ComponentType y;
        ComponentType z;

        /**
         * @brief Constructs a unit (1,1,1) vector.
         *
         * @return constexpr Vector3
         */
        static constexpr Vector3 unit()
        {
            return Vector3{ 1, 1, 1 };
        }

        /**
         * @brief Constructs a null vector.
         *
         * @return constexpr Vector3
         */
        static constexpr Vector3 zero()
        {
            return Vector3{ 0, 0, 0 };
        }

        /**
         * @brief Constructs an "up" vector (Z+).
         *
         * @return constexpr Vector3
         */
        static constexpr Vector3 up()
        {
            return Vector3{ 0, 0, 1 };
        }

        /**
         * @brief Constructs a vector where all components are set to value.
         *
         * @param value
         * @return constexpr Vector3
         */
        static constexpr Vector3 all(ComponentType value)
        {
            return Vector3{ value, value, value };
        }

        /**
         * @brief Constructs a vector from a vector collection of values.
         *
         * @return Vector3
         */
        static Vector3 from_vector(const std::vector<ComponentType> &);

        /**
         * @brief default constructor, constructs a null vector.
         */
        constexpr Vector3()
            : Vector3{ 0, 0, 0 }
        {}

        /**
         * @brief Full constructor.
         */
        constexpr Vector3(ComponentType x, ComponentType y, ComponentType z)
            : x{ x }
            , y{ y }
            , z{ z }
        {}

        /**
         * @brief Computes the dot product between self and other.
         *
         * @param other
         * @return ComponentType
         */
        ComponentType dot(const Vector3 &other) const;

        /**
         * @brief Computes the cross product between self and other.
         *
         * @param other
         * @return Vector3
         */
        Vector3 cross(const Vector3 &other) const;

        /**
         * @brief Computes a "reflected" direction vector with the given normal.
         *      normal
         *      \  |  ^
         * self  \ | / reflected
         *        v|/
         *     --------
         *
         * @param normal
         * @return Vector3
         */
        Vector3 reflect(const Vector3 &normal) const;

        /**
         * @brief Computes the euclidean norm of the vector.
         *
         * @return ComponentType
         */
        ComponentType norm() const;

        /**
         * @brief Computes the squared euclideant norm of the vector.
         * (equivalent to `self.dot(self)`)
         *
         * @return ComponentType
         */
        ComponentType square_norm() const;

        /**
         * @brief Computes a normalized vector out of self.
         * (equivalent to `self/self.norm()`)
         *
         * @return Vector3
         */
        Vector3 normalized() const;
        /**
         * @brief Linearly interpolates between this vector and other by factor.
         * If factor is 0, then the return value is equal to self, if factor is
         * 1, then the return value is equal to other.
         *
         * @param other
         * @param factor
         * @return Vector3
         */
        Vector3 lerp_to(const Vector3 &other, double factor) const;

        /**
         * @brief Adds a fourth component set to 1 to switch into projective
         * space
         *
         * @return Vector4
         */
        Vector4 to_projective() const;

        /**
         * @brief Component-wise addition assignment.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 &operator+=(const Vector3 &other);

        /**
         * @brief Component-wise subtraction assignment.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 &operator-=(const Vector3 &other);

        /**
         * @brief Multiplication of all components by a scalar.
         *
         * @param scalar
         * @return Vector3&
         */
        Vector3 &operator*=(const ComponentType &scalar);

        /**
         * @brief Division of all components by a scalar.
         *
         * @param scalar
         * @return Vector3&
         */
        Vector3 &operator/=(const ComponentType &scalar);

        /**
         * @brief Component-wise multiplication assignment.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 &operator*=(const Vector3 &other);

        /**
         * @brief Component-wise division assignment.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 &operator/=(const Vector3 &other);

        /**
         * @brief Component-wise addition.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 operator+(const Vector3 &other) const;
        /**
         * @brief Component-wise subtraction.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 operator-(const Vector3 &other) const;
        /**
         * @brief Multiplication of all components by a scalar.
         *
         * @param scalar
         * @return Vector3&
         */
        Vector3 operator*(const ComponentType &scalar) const;
        /**
         * @brief Division of all components by a scalar.
         *
         * @param scalar
         * @return Vector3&
         */
        Vector3 operator/(const ComponentType &scalar) const;
        /**
         * @brief Component-wise multiplication.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 operator*(const Vector3 &other) const;
        /**
         * @brief Component-wise division.
         *
         * @param other
         * @return Vector3&
         */
        Vector3 operator/(const Vector3 &other) const;
    };

    Vector3 operator*(const Vector3::ComponentType &scalar, const Vector3 vect);
    Vector3 operator-(const Vector3 &vect);

    /**
     * @brief Prints the vector on the output stream.
     * Affected by compact modifier.
     *
     * @param out
     * @param vect
     * @return std::ostream&
     */
    std::ostream &operator<<(std::ostream &out, const Vector3 &vect);
} // namespace pogl
