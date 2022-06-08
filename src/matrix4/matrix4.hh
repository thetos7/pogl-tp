#pragma once

#include <GL/gl.h>
#include <array>
#include <iosfwd>

namespace pogl
{
    class Matrix4
    {
    public:
        constexpr static auto DIM = 4;
        constexpr static auto ELEM_COUNT = DIM * DIM;
        using ElementType = GLfloat;
        using ElementsBufferType = std::array<ElementType, ELEM_COUNT>;

        /**
         * @brief Construct a new 4x4 Matrix filled with zeros
         *
         */
        Matrix4();

        /**
         * @brief Copy constructor
         *
         * @param other
         */
        Matrix4(const Matrix4 &other);

        /**
         * @brief Construct a new 4x4 Matrix filled with the values of elements
         *
         * @param elements The initial matrix values
         */
        Matrix4(ElementsBufferType elements);

        /**
         * @brief applies the matrix multiplication of self x rhs
         *
         * @param rhs Right hand side of matrix multiplication
         */
        void operator*=(const Matrix4 &rhs);

        /**
         * @brief computes the matrix multiplication of self x rhs and returns
         * new matrix
         *
         * @param rhs Right hand side of matrix multiplication
         */
        Matrix4 operator*(const Matrix4 &rhs) const;

        /**
         * @brief Creates a 4x4 identity matrix
         *
         * @return Matrix4
         */
        static Matrix4 identity();

        /**
         * @brief identity alias
         *
         * @return Matrix4
         */
        static inline Matrix4 eye()
        {
            return identity();
        }

        /**
         * @brief Gets a constant reference to the element in col, row
         *
         * @param col element column
         * @param row element row
         * @return const ElementType& constant reference to element
         */
        inline const ElementType &at(size_t col, size_t row) const
        {
            return elements_[row * DIM + col];
        }

        static Matrix4 frustum(float left, float right, float bottom, float top,
                               float znear, float zfar);

        static Matrix4 look_at(float eyeX, float eyeY, float eyeZ, float centerX,
                              float centerY, float centerZ, float upX,
                              float upY, float upZ);

        /**
         * @brief Gets a mutable reference to the element in col, row
         *
         * @param col element column
         * @param row element row
         * @return const ElementType& mutable reference to element
         */
        inline ElementType &at(size_t col, size_t row)
        {
            return elements_[row * DIM + col];
        }

        /**
         * @brief prints self onto output stream and returns stream
         *
         * @param out stream to print on
         * @param pretty whether to do pretty formatting or not
         * @return std::ostream& same as out
         */
        std::ostream &print(std::ostream &out, bool pretty) const;

        inline ElementsBufferType::iterator begin()
        {
            return elements_.begin();
        }
        inline ElementsBufferType::iterator end()
        {
            return elements_.end();
        }
        inline ElementsBufferType::const_iterator begin() const
        {
            return elements_.begin();
        }
        inline ElementsBufferType::const_iterator end() const
        {
            return elements_.end();
        }

    private:
        ElementsBufferType elements_;
    };

    std::ostream &operator<<(std::ostream &out, const Matrix4 &mat);

} // namespace pogl
