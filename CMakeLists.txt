cmake_minimum_required(VERSION 3.10)

project("OpenGL Sandbox"
    VERSION 1.0
    DESCRIPTION "OpenGL sandbox project for study purposes."
    LANGUAGES CXX)

# Specifies the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PROJECT_SOURCE_DIR}/src -I${PROJECT_SOURCE_DIR}/src/shared/layers")


add_subdirectory(src)
add_subdirectory(vendor)
