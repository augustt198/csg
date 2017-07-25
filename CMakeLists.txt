cmake_minimum_required(VERSION 2.8)

project(csg)

FILE(GLOB SOURCE_FILES src/*)

add_executable(
    csg
    ${SOURCE_FILES}
    deps/imgui/imgui.cpp
    deps/imgui/imgui_draw.cpp
    deps/imgui/imgui_impl_glfw_gl3.cpp
    deps/gl3w/src/gl3w.c
)

include_directories(include)
include_directories(deps/imgui)
include_directories(deps/gl3w/include)

add_definitions(-O3)

target_link_libraries(csg glfw3)
