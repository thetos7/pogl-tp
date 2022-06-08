#include <iostream>

#include "matrix4/matrix4.hh"

int main(/* int argc, char *argv[] */)
{
    using namespace pogl;
    const auto A = Matrix4(Matrix4::elements_buffer_type{
        1, 0, 0, 0, // l1
        2, 0, 0, 0, // l2
        3, 0, 0, 0, // l3
        4, 0, 0, 0 // l4
    });
    std::cout << "A:\n" << A << "\n";

    const auto B = Matrix4(Matrix4::elements_buffer_type{
        1, 2, 3, 4, // l1
        0, 0, 0, 0, // l2
        0, 0, 0, 0, // l3
        0, 0, 0, 0 // l4
    });
    std::cout << "B:\n" << B << "\n";

    const auto matmul_res = A * B;
    std::cout << "res:\n" << matmul_res << "\n";

    const auto frustum = Matrix4::frustum(-5, 5, -5, 5, 1, 100);
    std::cout << "frustum:\n" << frustum << "\n";
    return 0;
}
