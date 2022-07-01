#pragma once

#include <GL/gl.h>
#include <iosfwd>
#include <vector>

#include "vector3/fwd.hh"
#include "matrix4/matrix4.hh"

namespace pogl
{
    class Vector4
    {
    public:
        using ComponentType = GLfloat;

        ComponentType x;
        ComponentType y;
        ComponentType z;
        ComponentType w;

        /**
         * @brief Constructs a unit (1,1,1) vector.
         *
         * @return constexpr Vector4
         */
        static constexpr Vector4 unit()
        {
            return Vector4{ 1, 1, 1, 1 };
        }

        /**
         * @brief Constructs a null vector.
         *
         * @return constexpr Vector4
         */
        static constexpr Vector4 zero()
        {
            return Vector4{ 0, 0, 0, 0 };
        }

        /**
         * @brief Constructs an "up" vector (Z+).
         *
         * @return constexpr Vector4
         */
        static constexpr Vector4 up()
        {
            return Vector4{ 0, 0, 1, 1 };
        }

        /**
         * @brief Constructs a vector where all components are set to value.
         *
         * @param value
         * @return constexpr Vector4
         */
        static constexpr Vector4 all(ComponentType value)
        {
            return Vector4{ value, value, value, value };
        }

        /**
         * @brief Constructs a vector from a vector collection of values.
         *
         * @return Vector4
         */
        static Vector4 from_vector(const std::vector<ComponentType> &);

        /**
         * @brief default constructor, constructs a null vector.
         */
        constexpr Vector4()
            : Vector4{ 0, 0, 0, 0 }
        {}

        /**
         * @brief Full constructor.
         */
        constexpr Vector4(ComponentType x, ComponentType y, ComponentType z,
                          ComponentType w)
            : x{ x }
            , y{ y }
            , z{ z }
            , w{ w }
        {}

        /**
         * @brief Computes the dot product between self and other.
         *
         * @param other
         * @return ComponentType
         */
        ComponentType dot(const Vector4 &other) const;


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
         * @return Vector4
         */
        Vector4 normalized() const;

        Vector3 to_spatial() const;

        /**
         * @brief Component-wise addition assignment.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 &operator+=(const Vector4 &other);

        /**
         * @brief Component-wise subtraction assignment.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 &operator-=(const Vector4 &other);

        /**
         * @brief Multiplication of all components by a scalar.
         *
         * @param scalar
         * @return Vector4&
         */
        Vector4 &operator*=(const ComponentType &scalar);

        /**
         * @brief Division of all components by a scalar.
         *
         * @param scalar
         * @return Vector4&
         */
        Vector4 &operator/=(const ComponentType &scalar);

        /**
         * @brief Component-wise multiplication assignment.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 &operator*=(const Vector4 &other);

        /**
         * @brief Component-wise division assignment.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 &operator/=(const Vector4 &other);

        /**
         * @brief Component-wise addition.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 operator+(const Vector4 &other) const;
        /**
         * @brief Component-wise subtraction.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 operator-(const Vector4 &other) const;
        /**
         * @brief Multiplication of all components by a scalar.
         *
         * @param scalar
         * @return Vector4&
         */
        Vector4 operator*(const ComponentType &scalar) const;
        /**
         * @brief Division of all components by a scalar.
         *
         * @param scalar
         * @return Vector4&
         */
        Vector4 operator/(const ComponentType &scalar) const;
        /**
         * @brief Component-wise multiplication.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 operator*(const Vector4 &other) const;
        /**
         * @brief Component-wise division.
         *
         * @param other
         * @return Vector4&
         */
        Vector4 operator/(const Vector4 &other) const;
    };

    Vector4 operator*(const Matrix4 &transform, const Vector4 vect);
    Vector4 operator*(const Vector4::ComponentType &scalar, const Vector4 vect);
    Vector4 operator-(const Vector4 &vect);

    /**
     * @brief Prints the vector on the output stream.
     * Affected by compact modifier.
     *
     * @param out
     * @param vect
     * @return std::ostream&
     */
    std::ostream &operator<<(std::ostream &out, const Vector4 &vect);
} // namespace pogl
