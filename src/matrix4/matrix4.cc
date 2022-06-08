#include "matrix4.hh"

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "utils/utils.hh"

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
        std::fill(elements_.begin(), elements_.end(), 0);
    }

    Matrix4::Matrix4(const Matrix4 &other)
        : Matrix4(other.elements_)
    {}

    Matrix4::Matrix4(ElementsBufferType elements)
        : elements_(elements)
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
        this->elements_ = results;
    }

    Matrix4 Matrix4::operator*(const Matrix4 &rhs) const
    {
        Matrix4 copy = *this;
        copy *= rhs;
        return copy;
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

    Matrix4 Matrix4::look_at(float eyeX, float eyeY, float eyeZ, float centerX,
                            float centerY, float centerZ, float upX, float upY,
                            float upZ)
    {
        const auto fX = centerX-eyeX;
        const auto fY = centerY-eyeY;
        const auto fZ = centerZ-eyeZ;
        throw std::logic_error("Not implemented yet");
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
