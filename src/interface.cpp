//
// Created by Ember Lee on 4/23/25.
//

#include "interface.h"

#include <iostream>
#include <ostream>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "opendxf.h"

namespace opendxf::interface {
    GLFWwindow* window = nullptr;

    auto shader_vertex2d = "\
    #version 330 core\
    layout (location = 0) in vec2 pos;\
    uniform vec2 object_transform[2];\
    uniform vec4 world_transform;\
    \
    void main()\
    {\
        vec2 transformed = (object_transform[gl_VertexID] - world_transform.zw)*world_transform.xy;\
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
        -1., 1.,
        1., -1.,
        -1., 1.,
        1., 1.,
        1., -1.
    };

    float line_vertices[] = {
        -1., -1.,
        1., 1.,
    };

    // Graphics things
    unsigned int global_vertex_shader_id;
    unsigned int
    vaos[OBJECT_TYPE_MAX_VALUE],
    vbos[OBJECT_TYPE_MAX_VALUE],
    programs[OBJECT_TYPE_MAX_VALUE],
    global_ubo_locs[OBJECT_TYPE_MAX_VALUE],
    object_ubo_locs[OBJECT_TYPE_MAX_VALUE],
    draw_modes[OBJECT_TYPE_MAX_VALUE];

    unsigned int bg_vao, bg_vbo, bg_program;

    // Backend things
    unsigned int fbuf_width, fbuf_height;

    // Input things
    float view_x = 0.0f, view_y = 0.0f;
    double mouse_last_x = 0.0, mouse_last_y = 0.0;
    float sensitivity = 0.0025f;
    float scale = 1.0;
    bool plus_held = false, minus_held = false;

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

    void register_otype_resources(object_type type, float* vertices, size_t vertices_size, const char* shader_text, unsigned int draw_mode) {
        //Vbo/Vao
        glGenVertexArrays(1, &vaos[type]);
        glGenBuffers(1, &vbos[type]);

        glBindVertexArray(vaos[type]);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[type]);
        glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        programs[type] = createShaderProgram(shader_fragment2d);
        global_ubo_locs[type] = glGetUniformLocation(programs[type], "world_transform");
        object_ubo_locs[type] = glGetUniformLocation(programs[type], "object_transform");

        draw_modes[type] = draw_mode;
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

        global_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(global_vertex_shader_id, 1, &shader_vertex2d, nullptr);
        glCompileShader(global_vertex_shader_id);

        register_otype_resources(LINE, line_vertices, sizeof(line_vertices), shader_fragment2d, GL_LINES);

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    void quit() {
        glfwTerminate();
    }

    update_information update(void* _objects, unsigned int count) {
        glfwPollEvents();
        auto objects = static_cast<object*>(_objects);
        const bool running = !glfwWindowShouldClose(window);

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            const double diff_x = mouse_x - mouse_last_x;
            const double diff_y = mouse_y - mouse_last_y;
            view_x -= static_cast<float>(diff_x)*sensitivity/scale;
            view_y += static_cast<float>(diff_y)*sensitivity/scale;
        }
        mouse_last_x = mouse_x;
        mouse_last_y = mouse_y;

        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            if (!plus_held) {
                scale *= 2;
                plus_held = true;
            }
        } else {
            plus_held = false;
        }
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            if (!minus_held) {
                scale /= 2;
                minus_held = true;
            }
        } else {
            minus_held = false;
        }

        if (scale < 0.0) scale = 0.25;

        glClear(GL_COLOR_BUFFER_BIT);
        object_type last_bound_type = OBJECT_TYPE_MAX_VALUE;
        for (unsigned int i = 0; i < count; i++) {
            const auto& object = objects[i];
            const auto& type = object.type;
            const float world_transform[4] = {(static_cast<float>(fbuf_height) / static_cast<float>(fbuf_width))*scale, (1.0f)*scale, view_x, view_y};
            glUniform4fv(static_cast<GLint>(global_ubo_locs[type]), 1, world_transform);

            glUniform2fv(static_cast<GLint>(object_ubo_locs[type]), 2, reinterpret_cast<const float*>(&objects[i].pos_a));

            if (last_bound_type != type) {
                glUseProgram(programs[type]);
                glBindVertexArray(0);
                glBindVertexArray(vaos[type]);
                last_bound_type = type;
            }
            glDrawArrays(draw_modes[type], 0, 2);
        }

        glfwSwapBuffers(window);
        return {running};
    }
}
