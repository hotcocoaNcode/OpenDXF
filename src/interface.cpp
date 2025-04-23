//
// Created by Ember Lee on 4/23/25.
//

#include "interface.h"

#include <iostream>
#include <ostream>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

namespace opendxf {
    namespace interface {
        GLFWwindow* window = nullptr;

        void fbuf_callback(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }

        void init() {
            glfwInit();

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            window = glfwCreateWindow(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT, "OpenDXF", nullptr, nullptr);
            if (window == nullptr)
            {
                glfwTerminate();
                throw std::runtime_error("Couldn't create GLFW window");
            }
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, fbuf_callback);

            glViewport(0, 0, WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT);
        }

        void quit() {
            glfwTerminate();
        }

        update_information update() {
            glfwPollEvents();
            const bool running = !glfwWindowShouldClose(window);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            return {running};
        }
    }
}