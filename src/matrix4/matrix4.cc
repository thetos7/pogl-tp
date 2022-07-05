#include "matrix4.hh"

#include <cmath>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "utils/utils.hh"
#include "vector3/vector3.hh"

#define PI 3.14159265

namespace pogl
{
    using ElementType = Matrix4::ElementType;
    using ElementsBufferType = Matrix4::ElementsBufferType;

    inline ElementType &idx(ElementsBufferType &buf, size_t col, size_t row)
    {
        return buf[row * Matrix4::DIM + col];
    }

    Matrix4::Matrix4()
        : Matrix4(ElementsBufferType())
    {
        std::fill(_elements.begin(), _elements.end(), 0);
    }

    Matrix4::Matrix4(const Matrix4 &other)
        : Matrix4(other._elements)
    {}

    Matrix4::Matrix4(ElementsBufferType elements)
        : _elements(elements)
    {}

    void Matrix4::operator*=(const Matrix4 &rhs)
    {
        ElementsBufferType results{};
        ElementType sum;
        for (size_t y = 0; y < DIM; ++y)
        {
            for (size_t x = 0; x < DIM; ++x)
            {
                sum = 0;
                for (size_t i = 0; i < DIM; ++i)
                {
                    sum += this->at(i, y) * rhs.at(x, i);
                }
                idx(results, x, y) = sum;
            }
        }
        this->_elements = results;
    }

    Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
    {
        Matrix4 copy = *this;
        copy *= rhs;
        return copy;
    }

    Matrix4 &Matrix4::operator=(const Matrix4 &value)
    {
        _elements = value._elements;
        return *this;
    }

    Matrix4 Matrix4::identity()
    {
        ElementsBufferType elements{};
        std::fill(elements.begin(), elements.end(), 0);
        for (size_t i = 0; i < DIM; ++i)
        {
            idx(elements, i, i) = 1;
        }
        return Matrix4(elements);
    }

    Matrix4 Matrix4::transpose() const
    {
        return Matrix4(ElementsBufferType{
            at(0, 0), at(0, 1), at(0, 2), at(0, 3), // l1
            at(1, 0), at(1, 1), at(1, 2), at(1, 3), // l2
            at(2, 0), at(2, 1), at(2, 2), at(2, 3), // l3
            at(3, 0), at(3, 1), at(3, 2), at(3, 3) // l4
        });
    }

    Matrix4 &Matrix4::transpose_inplace()
    {
        ElementType tmp = 0;
        for (size_t i = 0; i < DIM - 1; ++i)
        {
            for (size_t j = i + 1; j < DIM; ++j)
            {
                tmp = at(i, j);
                at(i, j) = at(j, i);
                at(j, i) = tmp;
            }
        }
        return *this;
    }

    Matrix4 Matrix4::translation(ElementType x, ElementType y, ElementType z)
    {
        return Matrix4(Matrix4::ElementsBufferType{
            1, 0, 0, x, // l1
            0, 1, 0, y, // l2
            0, 0, 1, z, // l3
            0, 0, 0, 1 //  l4
        });
    }

    Matrix4 Matrix4::scale(ElementType factor)
    {
        return Matrix4(Matrix4::ElementsBufferType{
            factor, 0, 0, 0, // l1
            0, factor, 0, 0, // l2
            0, 0, factor, 0, // l3
            0, 0, 0, 1 //  l4
        });
    }

    Matrix4 Matrix4::rotate(float angle, char* axe)
    {
        auto param = angle * PI/180.0;
        if (axe == "x") {
            return Matrix4(Matrix4::ElementsBufferType{
                1, 0, 0, 0,                                    // l1
                0, (float) cos(param), (float) -sin(param), 0, // l2
                0, (float) sin(param), (float) cos(param), 0,  // l3
                0, 0, 0, 1                                     // l4
            });
        }
        else if (axe == "y") {
            return Matrix4(Matrix4::ElementsBufferType{
                (float) cos(param), 0, (float) sin(param), 0,   // l1
                0, 1, 0, 0,                                     // l2
                (float) -sin(param), 0, (float) cos(param), 0,  // l3
                0, 0, 0, 1                                      // l4
            });
        }
        else {
            return Matrix4(Matrix4::ElementsBufferType{
                (float) cos(param), (float) -sin(param), 0, 0,  // l1
                (float) sin(param), (float) cos(param), 0, 0,   // l2
                0, 0, 1, 0,                                     // l3
                0, 0, 0, 1                                      // l4
            });
        }
    }

    Matrix4 Matrix4::basis_change(const Vector3 &x_axis, const Vector3 &y_axis,
                                  const Vector3 &z_axis)
    {
        return Matrix4(Matrix4::ElementsBufferType{
            x_axis.x, y_axis.x, z_axis.x, 0, // l1
            x_axis.y, y_axis.y, z_axis.y, 0, // l2
            x_axis.z, y_axis.z, z_axis.z, 0, // l3
            0, 0, 0, 1 // l4
        });
    }

    Matrix4 Matrix4::frustum(float left, float right, float bottom, float top,
                             float znear, float zfar)
    {
        const auto E = (2 * znear) / (right - left);
        const auto F = (2 * znear) / (top - bottom);

        const auto A = (right + left) / (right - left);
        const auto B = (top + bottom) / (top - bottom);
        const auto C = -(zfar + znear) / (zfar - znear);
        const auto D = -(2 * zfar * znear) / (zfar - znear);

        return Matrix4(Matrix4::ElementsBufferType{
            E, 0, A, 0, // l1
            0, F, B, 0, // l2
            0, 0, C, D, // l3
            0, 0, -1, 0 // l4
        });
    }

    double cot(double x)
    {
        return std::cos(x) / std::sin(x);
    }

    Matrix4 Matrix4::perspective(float fovy, float aspect_ratio, float znear,
                                 float zfar)
    {
        const GLfloat f = cot(fovy / 2);
        const GLfloat A = f / aspect_ratio;
        const GLfloat B = (zfar + znear) / (znear - zfar);
        const GLfloat C = (2. * zfar * znear) / (znear - zfar);

        return Matrix4(Matrix4::ElementsBufferType{
            A, 0, 0, 0, // l1
            0, f, 0, 0, // l2
            0, 0, B, C, // l3
            0, 0, -1, 0 // l4
        });
    }

    Matrix4 Matrix4::look_at(float eyeX, float eyeY, float eyeZ, float centerX,
                             float centerY, float centerZ, float upX, float upY,
                             float upZ)
    {
        const auto look_direction =
            Vector3{
                centerX - eyeX,
                centerY - eyeY,
                centerZ - eyeZ,
            }
                .normalized();

        const auto up = Vector3{ upX, upY, upZ }.normalized();

        const auto right = look_direction.cross(up).normalized();
        const auto view_up = right.cross(look_direction).normalized();

        // comment auto formatting abuse to keep source code alignement
        const auto eyeTranslate = Matrix4::translation(-eyeX, -eyeY, -eyeZ);

        const auto rotation = Matrix4(Matrix4::ElementsBufferType{
            right.x /*     */, right.y /*     */, right.z /*     */, 0, // l1
            view_up.x /*   */, view_up.y /*   */, view_up.z /*   */, 0, // l2
            -look_direction.x, -look_direction.y, -look_direction.z, 0, // l3
            0 /*           */, 0 /*           */, 0 /*           */, 1 //  l4
        });

        return rotation * eyeTranslate;
    }

    std::ostream &pretty_print_mat(std::ostream &out, const Matrix4 &mat)
    {
        constexpr auto DIM = Matrix4::DIM;
        constexpr typeof(DIM) FIRST_LINE = 0;
        constexpr auto LAST_LINE = DIM - 1;

        constexpr char FILL = ' ';
        constexpr char LEFT_UPPER = '|'; // ⎡
        constexpr char LEFT_MIDDLE = '|'; // ⎢
        constexpr char LEFT_LOWER = '|'; // ⎣

        constexpr char RIGHT_UPPER = '|'; // ⎤
        constexpr char RIGHT_MIDDLE = '|'; // ⎥
        constexpr char RIGHT_LOWER = '|'; // ⎦

        size_t max_width = 0;
        for (const auto &n : mat)
        {
            std::ostringstream oss;
            oss << n;
            max_width = std::max(max_width, oss.str().length());
        }

        for (size_t y = 0; y < DIM; ++y)
        {
            if (y == FIRST_LINE)
            {
                out << LEFT_UPPER << " ";
            }
            else if (y == LAST_LINE)
            {
                out << LEFT_LOWER << " ";
            }
            else
            {
                out << LEFT_MIDDLE << " ";
            }

            for (size_t x = 0; x < DIM; ++x)
            {
                out << std::setw(max_width) << std::setfill(FILL)
                    << mat.at(x, y) << " ";
            }
            if (y == FIRST_LINE)
            {
                out << RIGHT_UPPER << "\n";
            }
            else if (y == LAST_LINE)
            {
                out << RIGHT_LOWER;
            }
            else
            {
                out << RIGHT_MIDDLE << "\n";
            }
        }
        return out;
    }

    std::ostream &print_mat(std::ostream &out, const Matrix4 &mat)
    {
        constexpr auto DIM = Matrix4::DIM;

        out << "[ ";
        for (size_t y = 0; y < DIM; ++y)
        {
            if (y != 0)
            {
                out << ',';
            }
            out << " [";
            for (size_t x = 0; x < DIM; ++x)
            {
                if (x != 0)
                {
                    out << ',';
                }
                out << ' ' << mat.at(x, y);
            }
            out << " ]";
        }
        out << " ]";
        return out;
    }

    std::ostream &Matrix4::print(std::ostream &out, bool pretty) const
    {
        if (pretty)
        {
            return pretty_print_mat(out, *this);
        }
        else
        {
            return print_mat(out, *this);
        }
    }

    std::ostream &operator<<(std::ostream &out, const Matrix4 &mat)
    {
        return mat.print(out, utils::pretty_print_enabled(out));
    }

} // namespace pogl
