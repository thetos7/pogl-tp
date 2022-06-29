#include <iostream>

#include "app/lifecycle.hh"

using namespace pogl;

int main(int argc, char *argv[])
{
    std::cout << "initialising GLFW...\n";
    init_glfw();
    std::cout << "initialising GLEW...\n";
    init_glew();
    std::cout << "initialising GL...\n";
    init_GL();
    std::cout << "initialising shaders...\n";
    init_shaders();
    std::cout << "initialising objects...\n";
    init_object();
    std::cout << "initialising POV...\n";
    init_POV();
    std::cout << "launching\n";
    std::cout << "Camera controls:\n";
    std::cout << "Z/S: Forward/Backwards\n";
    std::cout << "Q/D: Left/Right\n";
    std::cout << "Space/Left Shift: Up/Down\n";
    std::cout << "C: Toggle cursor capture\n";

    while (!glfwWindowShouldClose(window))
    {
        update();
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::cout << "exiting\n";

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
