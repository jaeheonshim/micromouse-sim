cmake_minimum_required(VERSION 3.14)
project(micromouse_sim)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB IMGUI_SRC
    "${CMAKE_SOURCE_DIR}/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/*.h"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/*.h"
)

add_executable(micromouse_sim
    src/main.cpp
    src/core/mouse.cpp
    src/ui/draw2d.cpp
    ${IMGUI_SRC}
)

target_include_directories(micromouse_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/imgui
)

target_link_libraries(micromouse_sim PRIVATE
  glfw
  OpenGL::GL
)