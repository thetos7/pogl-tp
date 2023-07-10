#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>

#include "engine/engine.hh"
#include "utils/log.hh"
#include "image/stb_image.h"

using namespace pogl;

std::string toupper(const std::string &in)
{
    std::string out(in.length(), '\0');
    std::transform(in.begin(), in.end(), out.begin(),
                   [](char c) { return std::toupper(c); });
    return out;
}

int main()
{
    std::srand(std::time(nullptr));
    stbi_set_flip_vertically_on_load(true);
    auto &engine = Engine::instance();
    engine.init();
    std::cout << LOG_INFO << "launching\n";
    std::cout << "Camera controls:\n";
    std::cout << toupper(glfwGetKeyName(GLFW_KEY_W, 0)) << "/"
              << toupper(glfwGetKeyName(GLFW_KEY_S, 0))
              << ": Forward/Backwards\n";
    std::cout << toupper(glfwGetKeyName(GLFW_KEY_Q, 0)) << "/"
              << toupper(glfwGetKeyName(GLFW_KEY_D, 0)) << ": Left/Right\n";
    std::cout << "Space/Left Shift: Up/Down\n";
    std::cout << toupper(glfwGetKeyName(GLFW_KEY_C, 0))
              << ": Toggle cursor capture\n";
    std::cout << "\n";
    std::cout << "Escape: Exit application\n";

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
