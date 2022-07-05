#include <cmath>
#include <iostream>

#include "engine/engine.hh"
#include "utils/log.hh"
#include "image/stb_image.h"

using namespace pogl;

int main()
{
    std::srand(std::time(nullptr));
    stbi_set_flip_vertically_on_load(true);
    auto &engine = Engine::instance();
    engine.init();
    std::cout << LOG_INFO << "launching\n";
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
