cmake_minimum_required(VERSION 3.14)
project(micromouse_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Source files ---
file(GLOB IMGUI_CORE_SRC
    ${CMAKE_SOURCE_DIR}/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/imgui/imgui_demo.cpp
)

set(APP_SRC
    src/main.cpp
    src/core/mouse.cpp
    src/core/world.cpp
    src/ui/draw2d.cpp
)

add_executable(micromouse_sim
    ${APP_SRC}
    ${IMGUI_CORE_SRC}
)

target_include_directories(micromouse_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
)

# =========================
# Platform-specific sections
# =========================

if (WIN32)
  message(STATUS "Building for Windows (DX11 + Win32)")

  target_sources(micromouse_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_win32.cpp
    ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_dx11.cpp
  )

  # ---- Link libraries ----
  if (MINGW)  # MSYS2/MinGW toolchains
    target_link_libraries(micromouse_sim PRIVATE
      d3d11 dxgi d3dcompiler
      dwmapi      # <- fixes __imp_Dwm* unresolved symbols
      imm32       # IME used by ImGui_ImplWin32
      user32 gdi32 shell32 ole32 oleaut32 uuid setupapi winmm
    )
  else()       # MSVC
    target_link_libraries(micromouse_sim PRIVATE
      d3d11 dxgi d3dcompiler
      dwmapi      # <- fixes __imp_Dwm* unresolved symbols
      imm32
    )
  endif()

  # Use WinMain and hide console window:
  set_property(TARGET micromouse_sim PROPERTY WIN32_EXECUTABLE TRUE)

  if (MSVC)
    target_compile_options(micromouse_sim PRIVATE /W4 /MP)
  endif()

else()
    message(STATUS "Building for Linux (GLFW + OpenGL3)")

    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    target_sources(micromouse_sim PRIVATE
        ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    )

    # Use GLFW’s loader (no GLAD or gl3w)
    target_compile_definitions(micromouse_sim PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLFW)

    target_link_libraries(micromouse_sim PRIVATE
        glfw
        OpenGL::GL
    )

    target_compile_options(micromouse_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================
# Optional
# =========================
enable_testing()
