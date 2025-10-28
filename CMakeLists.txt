cmake_minimum_required(VERSION 3.14)
project(micromouse_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB IMGUI_CORE_SRC
    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
)

set(NFD_SRC_DIR ${CMAKE_SOURCE_DIR}/nativefiledialog/src)

set(NFD_SOURCES
    ${NFD_SRC_DIR}/nfd_common.c
)

if (WIN32)
    list(APPEND NFD_SOURCES ${NFD_SRC_DIR}/nfd_win.cpp)
elseif(APPLE)
    list(APPEND NFD_SOURCES ${NFD_SRC_DIR}/nfd_cocoa.m)
else()
    list(APPEND NFD_SOURCES ${NFD_SRC_DIR}/nfd_gtk.c)
endif()

add_library(nfd STATIC ${NFD_SOURCES})

target_include_directories(nfd PUBLIC ${NFD_SRC_DIR}/include)

if (WIN32)
    target_link_libraries(nfd PRIVATE ole32 uuid comdlg32 shell32)
endif()

if (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    target_include_directories(nfd PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(nfd PRIVATE ${GTK3_LIBRARIES})
endif()

set(APP_SRC
    src/main.cpp
    src/core/mouse.cpp
    src/core/world.cpp
    src/ui/draw2d.cpp
    src/io/maze_io.cpp
)

add_executable(micromouse_sim
    ${APP_SRC}
    ${IMGUI_CORE_SRC}
)

target_include_directories(micromouse_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/io
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
)

# =========================
# Platform-specific setup
# =========================

if (WIN32)
  message(STATUS "Building for Windows (DX11 + Win32)")

  target_sources(micromouse_sim PRIVATE
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_win32.cpp
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp
  )

  if (MINGW)
    target_link_libraries(micromouse_sim PRIVATE
        d3d11 dxgi d3dcompiler dwmapi imm32
        user32 gdi32 shell32 ole32 oleaut32 uuid setupapi winmm
    )
  else()
    target_link_libraries(micromouse_sim PRIVATE
        d3d11 dxgi d3dcompiler dwmapi imm32
    )
  endif()

  set_property(TARGET micromouse_sim PROPERTY WIN32_EXECUTABLE TRUE)

  if (MSVC)
    target_compile_options(micromouse_sim PRIVATE /W4 /MP)
  endif()

elseif(APPLE)
  message(STATUS "Building for macOS (GLFW + OpenGL3)")

  find_package(OpenGL REQUIRED)
  find_package(glfw3 REQUIRED)

  target_sources(micromouse_sim PRIVATE
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
  )

  target_compile_definitions(micromouse_sim PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLFW)

  target_link_libraries(micromouse_sim PRIVATE
      glfw
      OpenGL::GL
      "-framework Cocoa"
      "-framework IOKit"
      "-framework CoreVideo"
  )

  target_compile_options(micromouse_sim PRIVATE -Wall -Wextra -Wpedantic)

else() # Linux or other UNIX
  message(STATUS "Building for Linux (GLFW + OpenGL3)")

  find_package(OpenGL REQUIRED)
  find_package(glfw3 REQUIRED)

  target_sources(micromouse_sim PRIVATE
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
      ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
  )

  target_compile_definitions(micromouse_sim PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLFW)

  target_link_libraries(micromouse_sim PRIVATE
      glfw
      OpenGL::GL
  )

  target_compile_options(micromouse_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()
