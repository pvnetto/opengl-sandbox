cmake_minimum_required(VERSION 3.10)

project("OpenGL Sandbox"
    VERSION 1.0
    DESCRIPTION "OpenGL sandbox project for study purposes."
    LANGUAGES CXX)

# Specifies the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(vendor)