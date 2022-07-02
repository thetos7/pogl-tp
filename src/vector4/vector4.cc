#include "vector4.hh"

#include <cmath>
#include <ostream>
#include <string>

#include "utils/utils.hh"
#include "vector3/vector3.hh"

namespace pogl
{
    using ComponentType = Vector4::ComponentType;

    Vector4 Vector4::from_vector(const std::vector<ComponentType> &v)
    {
        return Vector4{ v[0], v[1], v[2], v[3] };
    }

    ComponentType Vector4::square_norm() const
    {
        return this->dot(*this);
    }

    ComponentType Vector4::norm() const
    {
        return sqrt(square_norm());
    }

    Vector4 Vector4::normalized() const
    {
        auto norm_ = norm();
        if (norm_ != 0.)
        {
            return *this / norm_;
        }
        else
        {
            return zero();
        }
    }

    ComponentType Vector4::dot(const Vector4 &other) const
    {
        return this->x * other.x + this->y * other.y + this->z * other.z
            + this->w * other.w;
    }

    Vector3 Vector4::to_spatial() const
    {
        return Vector3(x / w, y / w, z / w);
    }

    std::vector<ComponentType> Vector4::as_vec()
    {
        return std::vector<ComponentType>{ x, y, z, w };
    }

    Vector4 &Vector4::operator+=(const Vector4 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector4 &Vector4::operator-=(const Vector4 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector4 &Vector4::operator*=(const ComponentType &scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector4 &Vector4::operator/=(const ComponentType &scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    Vector4 &Vector4::operator*=(const Vector4 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector4 &Vector4::operator/=(const Vector4 &other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector4 Vector4::operator*(const ComponentType &scalar) const
    {
        Vector4 copy = *this;
        copy *= scalar;
        return copy;
    }

    Vector4 Vector4::operator/(const ComponentType &scalar) const
    {
        Vector4 copy = *this;
        copy /= scalar;
        return copy;
    }

    Vector4 Vector4::operator*(const Vector4 &other) const
    {
        Vector4 copy = *this;
        copy *= other;
        return copy;
    }

    Vector4 Vector4::operator/(const Vector4 &other) const
    {
        Vector4 copy = *this;
        copy /= other;
        return copy;
    }

    Vector4 Vector4::operator+(const Vector4 &other) const
    {
        Vector4 copy = *this;
        copy += other;
        return copy;
    }

    Vector4 Vector4::operator-(const Vector4 &other) const
    {
        Vector4 copy = *this;
        copy -= other;
        return copy;
    }

    Vector4 operator*(const Matrix4 &transform, const Vector4 vect)
    {
        return Vector4{
            transform.at(0, 0) * vect.x + transform.at(1, 0) * vect.y
                + transform.at(2, 0) * vect.z + transform.at(3, 0) * vect.w,
            transform.at(0, 1) * vect.x + transform.at(1, 1) * vect.y
                + transform.at(2, 1) * vect.z + transform.at(3, 1) * vect.w,
            transform.at(0, 2) * vect.x + transform.at(1, 2) * vect.y
                + transform.at(2, 2) * vect.z + transform.at(3, 2) * vect.w,
            transform.at(0, 3) * vect.x + transform.at(1, 3) * vect.y
                + transform.at(2, 3) * vect.z + transform.at(3, 3) * vect.w
        };
    }

    Vector4 operator*(const ComponentType &scalar, const Vector4 vect)
    {
        return vect * scalar;
    }

    Vector4 operator-(const Vector4 &vect)
    {
        return Vector4{ -vect.x, -vect.y, -vect.z, -vect.w };
    }

    std::ostream &operator<<(std::ostream &out, const Vector4 &vect)
    {
        if (utils::compact_enabled(out))
        {
            return out << "(" << vect.x << ", " << vect.y << ", " << vect.z
                       << ", " << vect.w << ")";
        }
        return out << "Vector4(" << vect.x << ", " << vect.y << ", " << vect.z
                   << ", " << vect.w << ")";
    }

} // namespace pogl
