#include <iostream>

#include "engine/engine.hh"

using namespace pogl;

int main()
{
    auto &engine = Engine::instance();
    engine.init();
    std::cout << "launching\n";
    std::cout << "Camera controls:\n";
    std::cout << "Z/S: Forward/Backwards\n";
    std::cout << "Q/D: Left/Right\n";
    std::cout << "Space/Left Shift: Up/Down\n";
    std::cout << "C: Toggle cursor capture\n";
    while (!glfwWindowShouldClose(engine.window))
    {
        engine.update();
        engine.display();
        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }
    std::cout << "exiting\n";

    glfwDestroyWindow(engine.window);
    glfwTerminate();

    return 0;
}
