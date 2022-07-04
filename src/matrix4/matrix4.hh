#pragma once

#include <GL/gl.h>
#include <array>
#include <iosfwd>
#include <initializer_list>

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
         * @brief Copy assignement operator;
         * 
         * @param value 
         * @return Matrix4& 
         */
        Matrix4 &operator=(const Matrix4 &value);

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
         * @brief Creates a translation transform matrix.
         *
         * The created matrix has the form:
         *
         * ```
         * | 1 0 0 x |
         * | 0 1 0 y |
         * | 0 0 1 z |
         * | 0 0 0 1 |
         * ```
         *
         * @param x Value to translate by along the x axis
         * @param y Value to translate by along the y axis
         * @param z Value to translate by along the z axis
         * @return Matrix4 The translation transform matrix
         */
        static Matrix4 translation(ElementType x, ElementType y, ElementType z);

        /**
         * @brief Creates a scaling transform matrix.
         *
         * The created matrix has the form: (given f = factor)
         *
         * ```
         * | f 0 0 0 |
         * | 0 f 0 0 |
         * | 0 0 f 0 |
         * | 0 0 0 1 |
         * ```
         *
         * @param factor The scaling factor
         * @return Matrix4 The scaling transform matrix
         */
        static Matrix4 scale(ElementType factor);

        /**
         * @brief Gets a constant reference to the element in col, row
         *
         * @param col element column
         * @param row element row
         * @return const ElementType& constant reference to element
         */
        inline const ElementType &at(size_t col, size_t row) const
        {
            return _elements[row * DIM + col];
        }

        static Matrix4 frustum(float left, float right, float bottom, float top,
                               float znear, float zfar);

        static Matrix4 look_at(float eyeX, float eyeY, float eyeZ,
                               float centerX, float centerY, float centerZ,
                               float upX, float upY, float upZ);

        static Matrix4 perspective(float fovy, float aspect_ratio, float znear,
                                   float zfar);

        /**
         * @brief Gets a mutable reference to the element in col, row
         *
         * @param col element column
         * @param row element row
         * @return const ElementType& mutable reference to element
         */
        inline ElementType &at(size_t col, size_t row)
        {
            return _elements[row * DIM + col];
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
            return _elements.begin();
        }
        inline ElementsBufferType::iterator end()
        {
            return _elements.end();
        }
        inline ElementsBufferType::const_iterator begin() const
        {
            return _elements.begin();
        }
        inline ElementsBufferType::const_iterator end() const
        {
            return _elements.end();
        }

        /**
         * @brief Returns a pointer to the internal array of values
         *
         * @return const ElementType* pointer to the begining of the Matrix's
         * element array
         */
        inline const ElementType *data() const
        {
            return _elements.data();
        }

    private:
        ElementsBufferType _elements;
    };

    std::ostream &operator<<(std::ostream &out, const Matrix4 &mat);

} // namespace pogl
