//
// Created by Ember Lee on 4/23/25.
//

#include "interface.h"
#include <GLFW/glfw3.h>

namespace opendxf {
    namespace interface {
        GLFWwindow* window = nullptr;

        void init() {
            glfwInit();

            window = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
        }

        void quit() {
            glfwTerminate();
        }

        update_information update() {
            glfwPollEvents();
            return {!glfwWindowShouldClose(window)};
        }
    }
}