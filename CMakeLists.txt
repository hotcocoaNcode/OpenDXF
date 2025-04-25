cmake_minimum_required(VERSION 3.31)
project(OpenDXF)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

set(IMGUI_FILES
        src/imgui/imgui.cpp
        src/imgui/imgui_impl_glfw.cpp
        src/imgui/imgui_impl_opengl3.cpp
        src/imgui/imgui_tables.cpp
        src/imgui/imgui_widgets.cpp
        src/imgui/imgui_draw.cpp
)

add_executable(OpenDXF src/main.cpp
        src/interface.cpp
        src/opendxf.cpp
        ${IMGUI_FILES}
)

target_link_libraries(OpenDXF PRIVATE OpenGL::GL glm::glm glfw)