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

add_executable(OpenDXF src/main.cpp)
target_link_libraries(OpenDXF PRIVATE OpenGL::GL glm::glm glfw)