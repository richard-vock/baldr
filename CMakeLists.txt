cmake_minimum_required(VERSION 3.8.1)
project(baldr)

set (baldr_VERSION_MAJOR 0)
set (baldr_VERSION_MINOR 1)

set (OpenGL_GL_PREFERENCE "GLVND")

include_directories("${PROJECT_SOURCE_DIR}/include")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

option(DEBUG_VERBOSE "Verbose debugging output" OFF)
if (${DEBUG_VERBOSE})
    add_definitions(-DDEBUG_VERBOSE)
endif()

add_compile_options(-Wall -Wextra -Werror -fPIC -g)

add_library(baldr
    "src/data_buffer.cpp"
    "src/shader_interop.cpp"
    "src/shader_pipeline.cpp"
    "src/shader_program.cpp"
    "src/texture.cpp"
    "src/vertex_array.cpp"
    "src/framebuffer.cpp"
    "src/fullscreen_pass.cpp"
    "src/offscreen_context.cpp"
)
target_compile_features(baldr PUBLIC cxx_std_17)
target_link_libraries(baldr CONAN_PKG::eigen CONAN_PKG::fmt CONAN_PKG::glfw CONAN_PKG::glew "stdc++fs")

install (TARGETS baldr DESTINATION lib)
install (DIRECTORY include/ DESTINATION include/baldr)
