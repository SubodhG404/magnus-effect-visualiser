cmake_minimum_required(VERSION 3.16)
project(MagnusVisualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/backends)

set(SOURCES
    src/main.cpp
    src/ball.cpp
    src/simulation.cpp
    src/camera.cpp
    src/renderer.cpp
    src/trail.cpp
    src/input.cpp
)

set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_glut.cpp
    include/imgui/backends/imgui_impl_opengl2.cpp
)

add_executable(magnus_visualizer ${SOURCES} ${IMGUI_SOURCES})

target_link_libraries(magnus_visualizer
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    -lGLU
)
