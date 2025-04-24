//
// Created by Ember Lee on 4/23/25.
//

#include "interface.h"

#include <iostream>
#include <ostream>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

namespace opendxf::interface {
    GLFWwindow* window = nullptr;

    auto shader_vertex2d = "\
    #version 330 core\
    layout (location = 0) in vec2 pos;\
    uniform vec4 world_transform;\
    \
    void main()\
    {\
        vec2 transformed = pos*vec2(0.5)*world_transform.xy - world_transform.zw;\
        gl_Position = vec4(transformed.x, transformed.y, 0.0, 1.0);\
    }";

    auto shader_fragment2d = "\
    #version 330 core\
    out vec4 FragColor;\
    \
    void main()\
    {\
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\
    }";

    float quad_vertices[] = {
        -1., -1.,
        -1.,  1.,
         1., -1.,
        -1.,  1.,
         1.,  1.,
         1., -1.,
    };

    unsigned int global_vertex_shader_id;
    unsigned int program;
    unsigned int vao;
    unsigned int vbo;
    unsigned int ubo_location;

    unsigned int fbuf_width;
    unsigned int fbuf_height;

    void fbuf_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        fbuf_width = width;
        fbuf_height = height;
    }

    unsigned int createShaderProgram(const char* frag_text) {
        const unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_text, nullptr);
        glCompileShader(frag_shader);

        const unsigned int shader_program = glCreateProgram();
        glAttachShader(shader_program, global_vertex_shader_id);
        glAttachShader(shader_program, frag_shader);
        glLinkProgram(shader_program);
        glDeleteShader(frag_shader);

        return shader_program;
    }

    void init() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(_OPENDXF_WINDOW_BASE_WIDTH, _OPENDXF_WINDOW_BASE_HEIGHT, "OpenDXF", nullptr, nullptr);
        if (window == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("Couldn't create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, fbuf_callback);
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        fbuf_callback(window, w, h);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        global_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(global_vertex_shader_id, 1, &shader_vertex2d, nullptr);
        glCompileShader(global_vertex_shader_id);

        program = createShaderProgram(shader_fragment2d);

        glUseProgram(program);

        ubo_location = glGetUniformLocation(program, "world_transform");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void quit() {
        glfwTerminate();
    }

    update_information update() {
        glfwPollEvents();
        const bool running = !glfwWindowShouldClose(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float world_transform[4] = {static_cast<float>(fbuf_height) / static_cast<float>(fbuf_width), 1.0f, 0.0f, 0.0f};
        glUniform4fv(static_cast<GLint>(ubo_location), 1, world_transform);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        return {running};
    }
}