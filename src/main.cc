#include <iostream>

#include "matrix4/matrix4.hh"
#include "utils/utils.hh"

int main(/* int argc, char *argv[] */)
{
    using namespace pogl;
    const auto A = Matrix4(Matrix4::ElementsBufferType{
        1, 0, 0, 0, // l1
        2, 0, 0, 0, // l2
        3, 0, 0, 0, // l3
        4, 0, 0, 0 // l4
    });
    std::cout << "A:\n" << A << "\n";

    const auto B = Matrix4(Matrix4::ElementsBufferType{
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

    const auto look_at = Matrix4::look_at(5, 6, 7, 13, 14, 15, 1, 0, 0);
    std::cout << utils::pretty_print_on << "look_at:\n" << look_at <<"\n";
    return 0;
}
